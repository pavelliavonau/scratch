#ifndef USERDEBUGINFO_H
#define USERDEBUGINFO_H

#include <string>

class UserDebugInfo
{
public:
	UserDebugInfo(const std::string& info);
	~UserDebugInfo();

	static const std::string& userInfoRef();

private:
	static std::string userInfo;
};

#endif // USERDEBUGINFO_H
