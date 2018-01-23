#include "UserDebugInfo.h"

std::string UserDebugInfo::userInfo;

UserDebugInfo::UserDebugInfo(const std::string &info)
{
	this->userInfo = info;
}

UserDebugInfo::~UserDebugInfo()
{
	this->userInfo.clear();
}

const std::string &UserDebugInfo::userInfoRef()
{
	return userInfo;
}
