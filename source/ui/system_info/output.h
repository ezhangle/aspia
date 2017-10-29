//
// PROJECT:         Aspia Remote Desktop
// FILE:            ui/system_info/output.h
// LICENSE:         Mozilla Public License Version 2.0
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#ifndef _ASPIA_UI__SYSTEM_INFO__OUTPUT_H
#define _ASPIA_UI__SYSTEM_INFO__OUTPUT_H

//
// To display the information, we use a table representation. Each category is a separate table.
// Each table has a title and a header. Table header contains column titles.
// Categories fall into two types:
// 1. Generic table. Contains unlimited number of columns and rows.
// 2. Table of parameters / values. Contains only 2 columns: a parameter and a value.
//    Parameters can be merged into groups.
//
// The structure of the document is generated by calling methods sequentially.
//
// StartDocument()
//     StartTableGroup(<group_name>)
//         StartTable(<table_name>)
//             StartTableHeader()
//                 AddHeaderItem(<column_name1>, <column_width1>)
//                 ...
//                 AddHeaderItem(<column_nameN>, <column_widthN>)
//             EndTableHeader()
//             StartRow(<icon_id>)
//                 AddValue(<value1>, <unit1>)
//                 ...
//                 AddValue(<valueN>, <unitN>)
//             EndRow()
//         EndTable()
//     EndTableGroup()
//     StartTable(<table_name>)
//         StartTableHeader()
//             AddHeaderItem(<column_name1>, <column_width1>)
//             AddHeaderItem(<column_name2>, <column_width2>)
//         EndTableHeader()
//         StartGroup(<name>, <icon_id>)
//             AddParam(<icon_id1>, <parameter_name1>, <value1>, <unit1>)
//             ...
//             AddParam(<icon_idN>, <parameter_nameN>, <valueN>, <unitN>)
//         EndGroup()
//     EndTable()
// EndDocument()
//

#include "protocol/category.h"

#include <memory>
#include <string>

namespace aspia {

class OutputProxy;

class Output
{
public:
    Output();
    virtual ~Output();

    std::shared_ptr<OutputProxy> output_proxy() const { return proxy_; }

    class Table
    {
    public:
        Table(std::shared_ptr<OutputProxy> output, const std::string& name);
        ~Table();

    private:
        std::shared_ptr<OutputProxy> output_;
        DISALLOW_COPY_AND_ASSIGN(Table);
    };

    class TableHeader
    {
    public:
        TableHeader(std::shared_ptr<OutputProxy> output);
        ~TableHeader();

    private:
        std::shared_ptr<OutputProxy> output_;
        DISALLOW_COPY_AND_ASSIGN(TableHeader);
    };

    class Group
    {
    public:
        Group(std::shared_ptr<OutputProxy> output,
              const std::string& name,
              Category::IconId icon_id);
        ~Group();

    private:
        std::shared_ptr<OutputProxy> output_;
        DISALLOW_COPY_AND_ASSIGN(Group);
    };

    class Row
    {
    public:
        Row(std::shared_ptr<OutputProxy> output, Category::IconId icon_id);
        ~Row();

    private:
        std::shared_ptr<OutputProxy> output_;
        DISALLOW_COPY_AND_ASSIGN(Row);
    };

protected:
    virtual void StartDocument() = 0;
    virtual void EndDocument() = 0;

    virtual void StartTableGroup(const std::string& name) = 0;
    virtual void EndTableGroup() = 0;

    virtual void StartTable(const std::string& name) = 0;
    virtual void EndTable() = 0;

    virtual void StartTableHeader() = 0;
    virtual void EndTableHeader() = 0;
    virtual void AddHeaderItem(const std::string& name, int width) = 0;

    virtual void StartGroup(const std::string& name, Category::IconId icon_id) = 0;
    virtual void EndGroup() = 0;
    virtual void AddParam(Category::IconId icon_id,
                          const std::string& param,
                          const std::string& value,
                          const char* unit) = 0;

    virtual void StartRow(Category::IconId icon_id) = 0;
    virtual void EndRow() = 0;
    virtual void AddValue(const std::string& value, const char* unit) = 0;

private:
    friend class OutputProxy;

    std::shared_ptr<OutputProxy> proxy_;
};

} // namespace aspia

#endif // _ASPIA_UI__SYSTEM_INFO__OUTPUT_H