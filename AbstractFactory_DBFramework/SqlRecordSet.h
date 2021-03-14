#pragma once
#include <vector>
#include "RecordSet.h"
#include "SqlRecordSet.h"

class SqlRecordSet :
	public RecordSet
{
	const std::vector<std::string> m_Db{
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

	SqlRecordSet();
	const std::string& Get() override;
	bool HasNext() override;
};

