#include <lib.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <chrono>
#include <thread>


QString nm(int i)
{
	return QString::number(i);
}

std::ostream & operator <<(std::ostream & os, QString str)
{
	os << str.toStdString();
	return os;
}



namespace vk
{

QString getResponse(const QString & url)
{
	QNetworkAccessManager manager;
	QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url)));
	QEventLoop event;
	QObject::connect(response, SIGNAL(finished()),
					 &event, SLOT(quit()));
	event.exec();
	return response->readAll();
}

std::pair<QString, int> parseString(const QString & in_string, const QString & key, int from)
{
	const QString wrap = R"(")" + key + R"(":)";
	int newPos = in_string.indexOf(wrap, from);
	if(newPos == -1) { return std::make_pair(QString(), -1); }
	newPos +=  wrap.length();

	int comma = in_string.indexOf(QRegExp(R"([,}])"), newPos); /// end of value
	return std::make_pair(in_string.mid(newPos, comma - newPos),
						  comma + 1);
}

QString unlike(const QString & what,
			   const QString & item_id,
			   const QString &  own_id)
{
	/// https://vk.com/dev/likes.delete
	return api_site
			+ "likes.delete?"
			+ "type=" + what
			+ "&owner_id=" + own_id
			+ "&item_id=" + item_id
			+ "&v=5.68"
			+ "&access_token=" + access_token
			;
}

QString unlikeWithCaptcha(const QString & what,
						  const QString & item_id,
						  const QString &  own_id,
						  const QString & captcha_sid,
						  const QString & captcha_key)
{
	/// https://vk.com/dev/likes.delete
	/// https://vk.com/dev/captcha_error
	return api_site
			+ "likes.delete?"
			+ "type=" + what
			+ "&owner_id=" + own_id
			+ "&item_id=" + item_id
			+ "&captcha_sid=" + captcha_sid
			+ "&captcha_key=" + captcha_key
			+ "&v=5.68"
			+ "&access_token=" + access_token
			;
}

QString sendCaptcha(const QString & captha_url)
{
	/// https://vk.com/dev/messages.send
	return api_site
			+ "messages.send?"
			+ "user_id=" + my_id
			+ "&random_id=" + nm(rand() % 100000)
			+ "&peer_id=" + my_id
//			+ "&domain=" + domain // don't really know, what this field is for
			+ "&message=" + captha_url
			+ "&v=5.68"
			+ "&access_token=" + access_token
			;
}

void unlikeAllPhotos()
{
	/// https://vk.com/dev/fave.getPhotos
	auto get_liked_photos = [](QString num) -> QString
	{
		return api_site
				+ "fave.getPhotos?"
				+ "&offset=0"
				+ "&count=" + num
				+ "&photo_sizes=0"
				+ "&v=5.68"
				+ "&access_token=" + access_token
				;
	};
	QString liked_photos = getResponse(get_liked_photos("1"));

	auto countLim = parseString(liked_photos, "count", 0); /// how many photos are liked

	std::cout << "how many left:" << countLim.first << std::endl;
	liked_photos = getResponse(get_liked_photos(countLim.first)); /// get list of all

	unlikeAll(liked_photos, "photo", countLim.first.toInt());
}

void unlikeAllPosts()
{
	/// https://vk.com/dev/fave.getPosts
	/// says num should be <= 100
	int num = 100;
	auto get_liked_posts = [](QString num) -> QString
	{
		return api_site
				+ "fave.getPosts?"
				+ "&offset=0"
				+ "&count=" + num
				+ "&v=5.68"
				+ "&access_token=" + access_token
				;
	};
	QString liked_posts = getResponse(get_liked_posts(nm(num)));

	unlikeAll(liked_posts, "post", num);
}

void unlikeAll(const QString & response,
			   const QString & what,
			   int count)
{
	static int total_unliked = 0; /// just for visual output

	int currPos = 0;
	for(int i = 0; i < count; ++i)
	{
		auto item_id = parseString(response, "id", currPos);
		auto owner_id = parseString(response, "owner_id", item_id.second);
		if(what == "photo")
		{
			currPos = owner_id.second;
		}
		else //if(what == "post")
		{
			/// crutch
			currPos = response.indexOf("likes", owner_id.second);
			/// posts can have attachments with "id": and "owner_id": inside
			/// "likes" field is near the end of a post object
		}

		/// if not found
		if(item_id.first.isEmpty()) { return; }

//		std::cout << item_id.first << "\t" << owner_id.first << "\t";

		QString ans = getResponse(unlike(what, item_id.first, owner_id.first));

		/// crutch
		/// if captcha is requested there is an error code 14 - (https://vk.com/dev/captcha_error)
		while(ans.size() > short_response)
		{
			std::cout << "captcha" << std::endl;
//			std::cout << std::endl << " request:" << std::endl << ans << std::endl << std::endl;

			auto sid = parseString(ans, "captcha_sid", 0);			/// string with quotes inside
			/// delete first and last quote
			sid.first.chop(1); sid.first.remove(0, 1);

			auto img = parseString(ans, "captcha_img", sid.second);	/// string with quotes inside
			/// delete first and last quote
			img.first.chop(1); img.first.remove(0, 1);
			/// delete backslashes
			img.first.remove(R"(\)");
			/// now img.first is a path of captcha

			/// it will send captcha url to your chat with yourself
			/// you should send answer on captcha (key) to this chat

			QString msg_sent = getResponse(sendCaptcha(img.first));
//			std::cout << "message_sent: << msg_sent << std::endl << std::endl;
			auto message_id = parseString(msg_sent, "response", 0);

			/// get last message https://vk.com/dev/messages.get
			/// works good if you don't receive any other messages
			/// but you can modify it
			QString get_captcha_msg = api_site
									  + "messages.get?"
									  + "count=1"
									  + "&last_message_id=" + message_id.first
									  + "&v=5.68"
									  + "&access_token=" + access_token
									  ;

			/// wait for answer
			std::this_thread::sleep_for(std::chrono::seconds{20});
			auto captcha_msg = getResponse(get_captcha_msg);

			int wait_counter = 0;
			/// crutch
			/// if message is not found captcha_msg contains only total number of messages
			while(captcha_msg.size() < short_response)
			{
				std::this_thread::sleep_for(std::chrono::seconds{10}); /// wait more
				++wait_counter;
//				std::cout << "wait for captcha answer: "
//						  << int(wait_counter * 10 / 60. * 100) / 100.
//						  << " min" << std::endl;
				captcha_msg = getResponse(get_captcha_msg);

				/// here you can add "if wait_counter too big, wait for a very long time"
				/// and skip to the next
			}
//			std::cout << "message_get:" << captcha_msg << std::endl << std::endl;

			/// here we've got answer on captcha - it is in "body" field
			auto captcha_key = parseString(captcha_msg, "body", 0);
			/// chop quots
			captcha_key.first.chop(1); captcha_key.first.remove(0, 1);

			std::cout << "captcha_key = " << captcha_key.first << ";" << std::endl;

			QString reunlike = unlikeWithCaptcha(what,
												 item_id.first,
												 owner_id.first,
												 sid.first,
												 captcha_key.first
												 );
			/// if captcha answer is wrong - you'll get one more

			ans = getResponse(reunlike);
			std::cout << "reunlike(ans):";
		}
		std::cout << ans << "\t" << ++total_unliked << std::endl;

		/// between unlike attempts
		/// the more frequent, the more captchas
		std::this_thread::sleep_for(std::chrono::milliseconds{3000});
	}
}

} // end namespace vk
