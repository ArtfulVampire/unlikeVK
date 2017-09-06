#ifndef LIB_H
#define LIB_H

#include <QString>
#include <ios>
#include <iostream>


QString nm(int i);
std::ostream & operator <<(std::ostream & os, QString str);

namespace vk
{
const QString my_id = "";		/// your ID
const QString domain = "";		/// vk.com/domain, if you have this feature

const QString app_id = "";		/// your app ID - get instruction elsewhere
/// look https://vk.com/apps?act=manage and https://habrahabr.ru/post/265563/

/// DON'T SHOW ANYONE YOUR TOKEN
/// IT IS ENOUGH TO STEAL YOUR ACCOUNT
const QString access_token = "";

const int short_response = 42;	/// magic constant for a pair of crutches
const QString api_site = "https://api.vk.com/method/";

enum scopes
{
	notify		= 0x0000001,
	friends		= 0x0000002,
	photos		= 0x0000004,
	audio		= 0x0000008,
	video		= 0x0000010,
	/// blank	= 0x0000020,
	/// blank	= 0x0000040,
	pages		= 0x0000080,
	add_ref		= 0x0000100,
	/// blank	= 0x0000200,
	status		= 0x0000400,
	notes		= 0x0000800,
	messages	= 0x0001000,
	wall		= 0x0002000,
	/// blank	= 0x0004000,
	ads			= 0x0008000,
	offline		= 0x0010000,
	docs		= 0x0020000,
	groups		= 0x0040000,
	notifs		= 0x0080000,
	stats		= 0x0100000,
	/// blank	= 0x0200000,
	email		= 0x0400000,
	market		= 0x8000000 /// I dont use anyway
};


QString getResponse(const QString & url);

std::pair<QString, int> parseString(const QString & in_string,
									const QString & key,
									int from);

QString unlike(const QString & what,
			   const QString & item_id,
			   const QString & own_id);

QString unlikeWithCaptcha(const QString & what,
						  const QString & item_id,
						  const QString &  own_id,
						  const QString & captcha_sid,
						  const QString & captcha_key);


QString sendCaptcha(const QString & captha_url);

void unlikeAll(const QString & response,
			   const QString & what,
			   int count);

void unlikeAllPhotos();

void unlikeAllPosts();
}

#endif // LIB_H
