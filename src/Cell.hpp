/* mathCrossWordPuzzle
* Copyright 2019 Qusai Hroub.
*
* This file is part of the mathCrossWordPuzzle.
*
* Authors:
* Qusai Hroub <qusaihroub.r@gmail.com>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms and conditions of the GNU General Public License,
* version 3, as published by the Free Software Foundation.
*
* This program is distributed in the hope it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*/

#ifndef CELL
#define CELL

#include <string>

using namespace std;

namespace Cell {

class CoreCell {
private :
    static int m_lenght;
public:
    virtual string toString() = 0;
    virtual ~CoreCell();
    void setLength(int newLingth);
    int getLength();
    CoreCell(int len);
    CoreCell();
};

class ValuedCell : public CoreCell {
protected:
    string m_value;
    bool m_state = true;
public:
    virtual void setValue (string value) = 0;
    virtual string getValue() = 0;
    virtual void setState(bool s) = 0;
    ValuedCell();
    ValuedCell(int len);
    ValuedCell(string value, int len);
};

class BlockedCell : public CoreCell {
public:
    string toString();
};

class OperatorCell : public ValuedCell {
public:
    string toString ();
    string getValue ();
    void setValue ( string v);
    bool getState ();
    void setState ( bool s);
    OperatorCell (string __value, bool s);
    OperatorCell(string Value);
};

class NumericCell : public ValuedCell{
private:
    string m_gValue = "";
public:
    string toString ();
    string getValue ();
    void setValue ( string v);
    bool getState ();
    void setState ( bool s);
    NumericCell ();
    NumericCell (string Value);
    NumericCell (string Value, bool s);
    string getGValue ();
    void setGValue (string val);
};

}
#endif
