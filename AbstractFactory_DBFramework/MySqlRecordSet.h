#pragma once
#include <vector>
#include "RecordSet.h"

class MySqlRecordSet :
	public RecordSet
{
	const std::vector<std::string> m_Db{
"M",
"Y",
"S",
"Q",
"L",
"R",
"E",
"C",
"O",
"R",
"D",
"S"
	};
	std::vector<std::string>::const_iterator m_Cursor;
public:

	MySqlRecordSet();
	const std::string& Get() override;
	bool HasNext() override;
};

