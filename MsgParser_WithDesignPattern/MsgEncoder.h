#pragma once
#include <string>
#include <utility>
#include <vector>
#include "Messages.h"

struct Footer;
struct Data;
struct DataItem;
struct Header;

struct Visitor
{
	virtual ~Visitor() = default;

	virtual void visit(const Header& p) = 0;
	virtual void visit(const DataItem& p) = 0;
	virtual void visit(const Data& p) = 0;
	virtual void visit(const Footer& p) = 0;

	virtual std::string str() const = 0;
};

struct Element
{
	virtual ~Element() = default;
	virtual void accept(Visitor& v) const = 0;
};

struct TextMessage : Element
{
	std::string text1;
	std::string text2;

	explicit TextMessage(std::string&& text1, std::string&& text2)
		: text1(std::move(text1)), text2(std::move(text2))
	{
	}

	explicit TextMessage(const std::string& text1, const std::string& text2)
		: text1(text1), text2(text2)
	{
	}
};

struct Header : TextMessage
{
	explicit Header(std::string&& text1, std::string&& text2)
		: TextMessage(std::move(text1), std::move(text2))
	{
	}

	explicit Header(const std::string& text1, const std::string& text2)
		: TextMessage(text1, text2)
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct DataItem : TextMessage
{
	explicit DataItem(std::string&& text1, std::string&& text2)
		: TextMessage(std::move(text1), std::move(text2))
	{
	}

	explicit DataItem(const std::string& text1, const std::string& text2)
		: TextMessage(text1, text2)
	{
	}
	


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct Footer : TextMessage
{
	explicit Footer(std::string&& text1, std::string&& text2)
		: TextMessage(std::move(text1), std::move(text2))
	{
	}

	explicit Footer(const std::string& text1, const std::string& text2)
		: TextMessage(text1, text2)
	{
	}

	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct Data : std::vector<DataItem>, Element
{
	Data(const std::initializer_list<value_type>& _ilist)
		: std::vector<DataItem>(_ilist)
	{
	}

	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

