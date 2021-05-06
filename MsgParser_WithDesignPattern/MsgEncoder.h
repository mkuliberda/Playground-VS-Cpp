#pragma once
#include <string>
#include <utility>
#include <vector>
#include "Messages.h"

struct Footer;
struct Data;
struct DataItem;
struct Header;

struct Encoder
{
	virtual ~Encoder() = default;

	virtual void visit(const Header& p) = 0;
	virtual void visit(const DataItem& p) = 0;
	virtual void visit(const Data& p) = 0;
	virtual void visit(const Footer& p) = 0;

	virtual std::string str() const = 0;
};

struct Element
{
	virtual ~Element() = default;
	virtual void accept(Encoder& v) const = 0;
};

struct TextMessage : Element
{
	std::string part1;
	std::string part2;

	explicit TextMessage(std::string&& _part1, std::string&& _part2)
		: part1(std::move(_part1)), part2(std::move(_part2))
	{
	}

	explicit TextMessage(const std::string& _part1, const std::string& _part2)
		: part1(_part1), part2(_part2)
	{
	}
};

struct Header : TextMessage
{
	explicit Header(std::string&& _part1, std::string&& _part2)
		: TextMessage(std::move(_part1), std::move(_part2))
	{
	}

	explicit Header(const std::string& _part1, const std::string& _part2)
		: TextMessage(_part1, _part2)
	{
	}


	void accept(Encoder& v) const override
	{
		v.visit(*this);
	}
};

struct DataItem : TextMessage
{
	explicit DataItem(std::string&& _part1, std::string&& _part2)
		: TextMessage(std::move(_part1), std::move(_part2))
	{
	}

	explicit DataItem(const std::string& _part1, const std::string& _part2)
		: TextMessage(_part1, _part2)
	{
	}
	


	void accept(Encoder& v) const override
	{
		v.visit(*this);
	}
};

struct Footer : TextMessage
{
	explicit Footer(std::string&& _part1, std::string&& _part2)
		: TextMessage(std::move(_part1), std::move(_part2))
	{
	}

	explicit Footer(const std::string& _part1, const std::string& _part2)
		: TextMessage(_part1, _part2)
	{
	}

	void accept(Encoder& v) const override
	{
		v.visit(*this);
	}
};

struct Data : std::vector<DataItem>, Element
{
	Data(const std::initializer_list<value_type>& _init_list)
		: std::vector<DataItem>(_init_list)
	{
	}

	void accept(Encoder& v) const override
	{
		v.visit(*this);
	}
};

