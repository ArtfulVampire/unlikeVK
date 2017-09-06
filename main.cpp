#include <QCoreApplication>
#include <QString>
#include <iostream>
#include <lib.h>
#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	srand(time(NULL));

	int scope = vk::scopes::photos
				| vk::scopes::wall
				| vk::scopes::offline
				| vk::scopes::friends
				| vk::scopes::messages
//				| vk::scopes::audio
//				| vk::scopes::video
				;

	/// paste this string into browser address line
	/// agree with access rights
	/// token will appear in address line, copy it into access_token variable
	QString for_token = QString("https://oauth.vk.com/authorize?client_id=") + vk::app_id
						+ "&display=page"
						+ "&redirect_uri=https://oauth.vk.com/blank.html"
						+ "&scope=" + nm(scope)
						+ "&response_type=token"
						+ "&v=5.68";
//	std::cout << for_token << std::endl; return 0;


	QString get_liked_posts = vk::api_site
							 + "fave.getPosts?"
							 + "&offset=0"
							 + "&count=10"
							 + "&v=5.68"
							 + "&access_token=" + vk::access_token
							 ;

	QString liked_posts = vk::getResponse(get_liked_posts);
	std::cout << liked_posts << std::endl;

//	int numLeft = vk::parseString(liked_posts, "count", 0).first.toInt();
//	std::cout << "posts left: " << numLeft << std::endl; /// don't know why it is always 1000
//	while(numLeft > 100 && liked_posts.size() > vk::short_response)
//	{
//		vk::unlikeAllPosts();
//		liked_posts = vk::getResponse(get_liked_posts);
//		numLeft = vk::parseString(liked_posts, "count", 0).first.toInt();
//	}

	QString get_liked_photos = vk::api_site
							   + "fave.getPhotos?"
							   + "offset=0"
							   + "&count=100"
							   + "&v=5.68"
							   + "&access_token=" + vk::access_token
							   ;
	QString liked_photos = vk::getResponse(get_liked_photos);
	std::cout << liked_photos << std::endl;

//	vk::unlikeAllPhotos();


	return 0;
}
