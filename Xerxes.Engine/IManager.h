#pragma once

interface IManager
{
public:
	virtual void OnInit() = 0;
	virtual void OnShutdown() = 0;
protected:
	~IManager() = default;
};
