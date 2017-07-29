#pragma once
class Audio
{
public:
	static Audio& instance();

private:
	static Audio instance_;
	Audio();
	~Audio();
};

