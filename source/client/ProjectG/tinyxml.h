// Original: d:\build\custom\temp\code\client\projectg\tinyxml.h
// TinyXML 2.0-era (Lee Thomason), compiled into ProjectG without TIXML_USE_STL.
#ifndef PG_CLIENT_PROJECTG_TINYXML_H
#define PG_CLIENT_PROJECTG_TINYXML_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string>

#include "tinystr.h"

#define TIXML_STRING TiXmlString
#define TIXML_OSTREAM TiXmlOutStream

class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;

class TiXmlBase {
	friend class TiXmlNode;
	friend class TiXmlElement;
	friend class TiXmlDocument;

public:
	TiXmlBase() {
	}
	virtual ~TiXmlBase() {
	}

	virtual void Print(FILE* cfile, int depth) const = 0;

	static void SetCondenseWhiteSpace(bool condense) {
		condenseWhiteSpace = condense;
	}
	static bool IsWhiteSpaceCondensed() {
		return condenseWhiteSpace;
	}

	class StringToBuffer {
	public:
		StringToBuffer(const TIXML_STRING& str);
		~StringToBuffer();
		char* buffer; // 0x0
	};

protected:
	virtual void StreamOut(TIXML_OSTREAM*) const = 0;
	virtual const char* Parse(const char* p) = 0;

	static void PutString(const TIXML_STRING& str, TIXML_OSTREAM* out);
	static void PutString(const TIXML_STRING& str, TIXML_STRING* out);

	static const char* SkipWhiteSpace(const char* p);
	static const char* ReadName(const char* p, TIXML_STRING* name);
	static const char* ReadText(const char* in, TIXML_STRING* text, bool ignoreWhiteSpace, const char* endTag, bool ignoreCase);
	static const char* GetEntity(const char* in, char* value);
	inline static const char* GetChar(const char* p, char* value) {
		assert(p);
		if (*p == '&') {
			return GetEntity(p, value);
		} else {
			*value = *p;
			return p + 1;
		}
	}
	static bool StringEqual(const char* p, const char* endTag, bool ignoreCase);

	enum {
		TIXML_NO_ERROR = 0,
		TIXML_ERROR,
		TIXML_ERROR_OPENING_FILE,
		TIXML_ERROR_OUT_OF_MEMORY,
		TIXML_ERROR_PARSING_ELEMENT,
		TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TIXML_ERROR_READING_ELEMENT_VALUE,
		TIXML_ERROR_READING_ATTRIBUTES,
		TIXML_ERROR_PARSING_EMPTY,
		TIXML_ERROR_READING_END_TAG,
		TIXML_ERROR_PARSING_UNKNOWN,
		TIXML_ERROR_PARSING_COMMENT,
		TIXML_ERROR_PARSING_DECLARATION,
		TIXML_ERROR_DOCUMENT_EMPTY,

		TIXML_ERROR_STRING_COUNT
	};
	static const char* errorMsg[TIXML_ERROR_STRING_COUNT];

private:
	struct Entity {
		const char* str; // 0x0
		unsigned int strLength; // 0x4
		char chr; // 0x8
	};
	enum {
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6
	};
	static Entity entity[NUM_ENTITY];
	static bool condenseWhiteSpace;
};
CHECK_SIZE(TiXmlBase, 0x4);

class TiXmlNode : public TiXmlBase {
	LAYOUT_CHECK_FRIEND;
	friend class TiXmlDocument;
	friend class TiXmlElement;
	friend TIXML_OSTREAM& operator<<(TIXML_OSTREAM& out, const TiXmlNode& base);

public:
	enum NodeType {
		DOCUMENT,
		ELEMENT,
		COMMENT,
		UNKNOWN,
		TEXT,
		DECLARATION,
		TYPECOUNT
	};

	virtual ~TiXmlNode();

	const char* Value() const {
		return value.c_str();
	}
	void SetValue(const char* _value) {
		value = _value;
	}

	void Clear();

	TiXmlNode* Parent() const {
		return parent;
	}

	TiXmlNode* FirstChild() const {
		return firstChild;
	}
	TiXmlNode* FirstChild(const char* value) const;

	TiXmlNode* LastChild() const {
		return lastChild;
	}
	TiXmlNode* LastChild(const char* value) const;

	TiXmlNode* IterateChildren(TiXmlNode* previous) const;
	TiXmlNode* IterateChildren(const char* value, TiXmlNode* previous) const;

	TiXmlNode* InsertEndChild(const TiXmlNode& addThis);
	TiXmlNode* InsertBeforeChild(TiXmlNode* beforeThis, const TiXmlNode& addThis);
	TiXmlNode* InsertAfterChild(TiXmlNode* afterThis, const TiXmlNode& addThis);
	TiXmlNode* ReplaceChild(TiXmlNode* replaceThis, const TiXmlNode& withThis);
	bool RemoveChild(TiXmlNode* removeThis);

	TiXmlNode* PreviousSibling() const {
		return prev;
	}
	TiXmlNode* PreviousSibling(const char*) const;

	TiXmlNode* NextSibling() const {
		return next;
	}
	TiXmlNode* NextSibling(const char*) const;

	TiXmlElement* NextSiblingElement() const;
	TiXmlElement* NextSiblingElement(const char*) const;

	TiXmlElement* FirstChildElement() const;
	TiXmlElement* FirstChildElement(const char* value) const;

	virtual int Type() const {
		return type;
	}

	TiXmlDocument* GetDocument() const;

	bool NoChildren() const {
		return !firstChild;
	}

	TiXmlDocument* ToDocument() const {
		return (this && type == DOCUMENT) ? (TiXmlDocument*)this : 0;
	}
	TiXmlElement* ToElement() const {
		return (this && type == ELEMENT) ? (TiXmlElement*)this : 0;
	}
	TiXmlComment* ToComment() const {
		return (this && type == COMMENT) ? (TiXmlComment*)this : 0;
	}
	TiXmlUnknown* ToUnknown() const {
		return (this && type == UNKNOWN) ? (TiXmlUnknown*)this : 0;
	}
	TiXmlText* ToText() const {
		return (this && type == TEXT) ? (TiXmlText*)this : 0;
	}
	TiXmlDeclaration* ToDeclaration() const {
		return (this && type == DECLARATION) ? (TiXmlDeclaration*)this : 0;
	}

	virtual TiXmlNode* Clone() const = 0;

protected:
	TiXmlNode(NodeType type);

	// Copy the value and the user data into a clone.
	void CopyToClone(TiXmlNode* target) const {
		target->SetValue(value.c_str());
		target->userData = userData;
	}

	TiXmlNode* LinkEndChild(TiXmlNode* addThis);

	TiXmlNode* Identify(const char* start);

	TIXML_STRING SValue() const {
		return value;
	}

	TiXmlNode* parent; // 0x4
	NodeType type; // 0x8

	TiXmlNode* firstChild; // 0xc
	TiXmlNode* lastChild; // 0x10

	TIXML_STRING value; // 0x14

	TiXmlNode* prev; // 0x1c
	TiXmlNode* next; // 0x20
	void* userData; // 0x24
};
CHECK_SIZE(TiXmlNode, 0x28);
BEGIN_LAYOUT_CHECK(TiXmlNode)
	CHECK_FIELD(TiXmlNode, parent, 0x4);
	CHECK_FIELD(TiXmlNode, type, 0x8);
	CHECK_FIELD(TiXmlNode, firstChild, 0xc);
	CHECK_FIELD(TiXmlNode, lastChild, 0x10);
	CHECK_FIELD(TiXmlNode, value, 0x14);
	CHECK_FIELD(TiXmlNode, prev, 0x1c);
	CHECK_FIELD(TiXmlNode, next, 0x20);
	CHECK_FIELD(TiXmlNode, userData, 0x24);
END_LAYOUT_CHECK

class TiXmlAttribute : public TiXmlBase {
	LAYOUT_CHECK_FRIEND;
	friend class TiXmlAttributeSet;

public:
	TiXmlAttribute() : prev(0), next(0) {
	}
	TiXmlAttribute(const char* _name, const char* _value) : name(_name), value(_value), prev(0), next(0) {
	}

	const char* Name() const {
		return name.c_str();
	}
	const char* Value() const {
		return value.c_str();
	}
	const int IntValue() const;
	const double DoubleValue() const;

	void SetName(const char* _name) {
		name = _name;
	}
	void SetValue(const char* _value) {
		value = _value;
	}

	void SetIntValue(int value);
	void SetDoubleValue(double value);

	TiXmlAttribute* Next() const;
	TiXmlAttribute* Previous() const;

	bool operator==(const TiXmlAttribute& rhs) const {
		return rhs.name == name;
	}
	bool operator<(const TiXmlAttribute& rhs) const {
		return name < rhs.name;
	}
	bool operator>(const TiXmlAttribute& rhs) const {
		return name > rhs.name;
	}

	virtual void Print(FILE* cfile, int depth) const;
	virtual void StreamOut(TIXML_OSTREAM* out) const;
	virtual const char* Parse(const char* p);

	void SetDocument(TiXmlDocument* doc) {
		document = doc;
	}

private:
	TiXmlDocument* document; // 0x4
	TIXML_STRING name; // 0x8
	TIXML_STRING value; // 0x10
	TiXmlAttribute* prev; // 0x18
	TiXmlAttribute* next; // 0x1c
};
CHECK_SIZE(TiXmlAttribute, 0x20);
BEGIN_LAYOUT_CHECK(TiXmlAttribute)
	CHECK_FIELD(TiXmlAttribute, document, 0x4);
	CHECK_FIELD(TiXmlAttribute, name, 0x8);
	CHECK_FIELD(TiXmlAttribute, value, 0x10);
	CHECK_FIELD(TiXmlAttribute, prev, 0x18);
	CHECK_FIELD(TiXmlAttribute, next, 0x1c);
END_LAYOUT_CHECK

class TiXmlAttributeSet {
	LAYOUT_CHECK_FRIEND;

public:
	TiXmlAttributeSet();
	~TiXmlAttributeSet();

	void Add(TiXmlAttribute* attribute);
	void Remove(TiXmlAttribute* attribute);

	TiXmlAttribute* First() const {
		return (sentinel.next == &sentinel) ? 0 : sentinel.next;
	}
	TiXmlAttribute* Last() const {
		return (sentinel.prev == &sentinel) ? 0 : sentinel.prev;
	}
	TiXmlAttribute* Find(const char* name) const;

private:
	TiXmlAttribute sentinel; // 0x0
};
CHECK_SIZE(TiXmlAttributeSet, 0x20);

class TiXmlElement : public TiXmlNode {
	LAYOUT_CHECK_FRIEND;

public:
	TiXmlElement(const char* in_value);

	virtual ~TiXmlElement();

	std::string Attribute(const char* name) const;
	const char* Attribute(const char* name, int* i) const;

	void SetAttribute(const char* name, const char* value);
	void SetAttribute(const char* name, int value);

	void RemoveAttribute(const char* name);

	TiXmlAttribute* FirstAttribute() const {
		return attributeSet.First();
	}
	TiXmlAttribute* LastAttribute() const {
		return attributeSet.Last();
	}

	virtual TiXmlNode* Clone() const;
	virtual void Print(FILE* cfile, int depth) const;

protected:
	virtual void StreamOut(TIXML_OSTREAM* out) const;
	virtual const char* Parse(const char* p);

	const char* ReadValue(const char* in);

private:
	TiXmlAttributeSet attributeSet; // 0x28
};
CHECK_SIZE(TiXmlElement, 0x48);

class TiXmlComment : public TiXmlNode {
public:
	TiXmlComment() : TiXmlNode(TiXmlNode::COMMENT) {
	}
	virtual ~TiXmlComment() {
	}

	virtual TiXmlNode* Clone() const;
	virtual void Print(FILE* cfile, int depth) const;

protected:
	virtual void StreamOut(TIXML_OSTREAM* out) const;
	virtual const char* Parse(const char* p);
};
CHECK_SIZE(TiXmlComment, 0x28);

class TiXmlText : public TiXmlNode {
	friend class TiXmlElement;

public:
	TiXmlText(const char* initValue) : TiXmlNode(TiXmlNode::TEXT) {
		SetValue(initValue);
	}
	virtual ~TiXmlText() {
	}

protected:
	virtual TiXmlNode* Clone() const;
	virtual void Print(FILE* cfile, int depth) const;
	virtual void StreamOut(TIXML_OSTREAM* out) const;
	virtual const char* Parse(const char* p);

	bool Blank() const;
};
CHECK_SIZE(TiXmlText, 0x28);

class TiXmlDeclaration : public TiXmlNode {
	LAYOUT_CHECK_FRIEND;

public:
	TiXmlDeclaration() : TiXmlNode(TiXmlNode::DECLARATION) {
	}
	TiXmlDeclaration(const char* _version, const char* _encoding, const char* _standalone);
	virtual ~TiXmlDeclaration() {
	}

	const char* Version() const {
		return version.c_str();
	}
	const char* Encoding() const {
		return encoding.c_str();
	}
	const char* Standalone() const {
		return standalone.c_str();
	}

	virtual TiXmlNode* Clone() const;
	virtual void Print(FILE* cfile, int depth) const;

protected:
	virtual void StreamOut(TIXML_OSTREAM* out) const;
	virtual const char* Parse(const char* p);

private:
	TIXML_STRING version; // 0x28
	TIXML_STRING encoding; // 0x30
	TIXML_STRING standalone; // 0x38
};
CHECK_SIZE(TiXmlDeclaration, 0x40);

class TiXmlUnknown : public TiXmlNode {
public:
	TiXmlUnknown() : TiXmlNode(TiXmlNode::UNKNOWN) {
	}
	virtual ~TiXmlUnknown() {
	}

	virtual TiXmlNode* Clone() const;
	virtual void Print(FILE* cfile, int depth) const;

protected:
	virtual void StreamOut(TIXML_OSTREAM* out) const;
	virtual const char* Parse(const char* p);
};
CHECK_SIZE(TiXmlUnknown, 0x28);

class TiXmlDocument : public TiXmlNode {
	LAYOUT_CHECK_FRIEND;

public:
	TiXmlDocument();
	TiXmlDocument(const char* documentName);

	virtual ~TiXmlDocument() {
	}

	bool LoadFile();
	bool SaveFile() const;
	bool LoadFile(const char* filename);
	bool SaveFile(const char* filename) const;
	bool LoadFileEx(const char* filename);
	bool LoadFileFromMemoryEx(const char* data);

	TiXmlElement* RootElement() const {
		return FirstChildElement();
	}

	bool Error() const {
		return error;
	}
	const char* ErrorDesc() const {
		return errorDesc.c_str();
	}
	const int ErrorId() const {
		return errorId;
	}
	void ClearError() {
		error = false;
		errorId = 0;
		errorDesc = "";
	}

	void Print() const {
		Print(stdout, 0);
	}
	virtual void Print(FILE* cfile, int depth = 0) const;
	virtual const char* Parse(const char* p);

	void SetError(int err) {
		assert(err > 0 && err < TIXML_ERROR_STRING_COUNT);
		error = true;
		errorId = err;
		errorDesc = errorMsg[errorId];
	}

protected:
	virtual void StreamOut(TIXML_OSTREAM* out) const;
	virtual TiXmlNode* Clone() const;

private:
	bool error; // 0x28
	int errorId; // 0x2c
	TIXML_STRING errorDesc; // 0x30
};
CHECK_SIZE(TiXmlDocument, 0x38);
BEGIN_LAYOUT_CHECK(TiXmlDocument)
	CHECK_FIELD(TiXmlDocument, error, 0x28);
	CHECK_FIELD(TiXmlDocument, errorId, 0x2c);
	CHECK_FIELD(TiXmlDocument, errorDesc, 0x30);
END_LAYOUT_CHECK

TIXML_OSTREAM& operator<<(TIXML_OSTREAM& out, const TiXmlNode& base);

#endif // PG_CLIENT_PROJECTG_TINYXML_H
