#pragma once

interface IInspectorDrawer
{
public:
	virtual void OnInspector() = 0;

protected:
	~IInspectorDrawer() = default;
};