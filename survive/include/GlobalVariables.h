#pragma once

class GlobalVariables
{
private:
	bool _drawCircle;
	GlobalVariables* _instance;

	GlobalVariables();
	~GlobalVariables() {}

	GlobalVariables(const GlobalVariables& g);
	GlobalVariables& operator=(GlobalVariables const&);

public:
	bool getVal() { return _drawCircle; }
	void setVal(bool val) { _drawCircle = val; }

	static GlobalVariables* getInstance() 
	{
		static GlobalVariables instance;
		return &instance;
	}
};
