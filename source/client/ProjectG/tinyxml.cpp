// Original: d:\build\custom\temp\code\client\projectg\tinyxml.cpp
#include "tinyxml.h"

#include <locale.h>

#include "westpak.h"
#include "wresrcmng.h"

void __cdecl CloseCFile(cFile* file);

// tinyxml.cpp:64
void TiXmlBase::PutString(const TIXML_STRING& str, TIXML_OSTREAM* stream) {
	TIXML_STRING buffer;
	PutString(str, &buffer);
	(*stream) << buffer;
}

// tinyxml.cpp:71
void TiXmlBase::PutString(const TIXML_STRING& str, TIXML_STRING* outString) {
	setlocale(LC_CTYPE, "Kor");

	int i = 0;
	int length = (int)str.length();
	while (i < length) {
		int c = str[i];

		if (c == '&'
			&& i < (length - 2)
			&& str[i + 1] == '#'
			&& str[i + 2] == 'x') {
			// Hexadecimal character reference: pass through unchanged.
			while (i < length) {
				outString->append(str.c_str() + i, 1);
				++i;
				if (str[i] == ';')
					break;
			}
		} else if (c == '&') {
			outString->append(entity[0].str, entity[0].strLength);
			++i;
		} else if (c == '<') {
			outString->append(entity[1].str, entity[1].strLength);
			++i;
		} else if (c == '>') {
			outString->append(entity[2].str, entity[2].strLength);
			++i;
		} else if (c == '\"') {
			outString->append(entity[3].str, entity[3].strLength);
			++i;
		} else if (c == '\'') {
			outString->append(entity[4].str, entity[4].strLength);
			++i;
		} else if (isleadbyte((unsigned char)c) && i < length - 1) {
			// Double-byte character: copy both bytes unchanged.
			outString->append(&str[i], 2);
			i += 2;
		} else if (c < 32 || c > 126) {
			char buf[32];
			sprintf(buf, "&#x%02X;", (unsigned)(c & 0xff));
			outString->append(buf, strlen(buf));
			++i;
		} else {
			char realc = (char)c;
			outString->append(&realc, 1);
			++i;
		}
	}
}

// tinyxml.cpp:156
TiXmlBase::StringToBuffer::StringToBuffer(const TIXML_STRING& str) {
	buffer = new char[str.length() + 1];
	if (buffer) {
		strcpy(buffer, str.c_str());
	}
}

// tinyxml.cpp:166
TiXmlBase::StringToBuffer::~StringToBuffer() {
	delete[] buffer;
}

// tinyxml.cpp:173
TiXmlNode::TiXmlNode(NodeType _type) {
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
	userData = 0;
}

// tinyxml.cpp:185
TiXmlNode::~TiXmlNode() {
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while (node) {
		temp = node;
		node = node->next;
		delete temp;
	}
}

// tinyxml.cpp:199
void TiXmlNode::Clear() {
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while (node) {
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = 0;
	lastChild = 0;
}

// tinyxml.cpp:216
TiXmlNode* TiXmlNode::LinkEndChild(TiXmlNode* node) {
	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if (lastChild)
		lastChild->next = node;
	else
		firstChild = node;

	lastChild = node;
	return node;
}

// tinyxml.cpp:233
TiXmlNode* TiXmlNode::InsertEndChild(const TiXmlNode& addThis) {
	TiXmlNode* node = addThis.Clone();
	if (!node)
		return 0;

	return LinkEndChild(node);
}

// tinyxml.cpp:243
TiXmlNode* TiXmlNode::InsertBeforeChild(TiXmlNode* beforeThis, const TiXmlNode& addThis) {
	if (!beforeThis || beforeThis->parent != this)
		return 0;

	TiXmlNode* node = addThis.Clone();
	if (!node)
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if (beforeThis->prev) {
		beforeThis->prev->next = node;
	} else {
		assert(firstChild == beforeThis);
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}

// tinyxml.cpp:269
TiXmlNode* TiXmlNode::InsertAfterChild(TiXmlNode* afterThis, const TiXmlNode& addThis) {
	if (!afterThis || afterThis->parent != this)
		return 0;

	TiXmlNode* node = addThis.Clone();
	if (!node)
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if (afterThis->next) {
		afterThis->next->prev = node;
	} else {
		assert(lastChild == afterThis);
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}

// tinyxml.cpp:295
TiXmlNode* TiXmlNode::ReplaceChild(TiXmlNode* replaceThis, const TiXmlNode& withThis) {
	if (replaceThis->parent != this)
		return 0;

	TiXmlNode* node = withThis.Clone();
	if (!node)
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if (replaceThis->next)
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if (replaceThis->prev)
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}

// tinyxml.cpp:323
bool TiXmlNode::RemoveChild(TiXmlNode* removeThis) {
	if (removeThis->parent != this) {
		assert(0);
		return false;
	}

	if (removeThis->next)
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if (removeThis->prev)
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

// tinyxml.cpp:345
TiXmlNode* TiXmlNode::FirstChild(const char* value) const {
	TiXmlNode* node;
	for (node = firstChild; node; node = node->next) {
		if (node->SValue() == TIXML_STRING(value))
			return node;
	}
	return 0;
}

// tinyxml.cpp:356
TiXmlNode* TiXmlNode::LastChild(const char* value) const {
	TiXmlNode* node;
	for (node = lastChild; node; node = node->prev) {
		if (node->SValue() == TIXML_STRING(value))
			return node;
	}
	return 0;
}

// tinyxml.cpp:367
TiXmlNode* TiXmlNode::IterateChildren(TiXmlNode* previous) const {
	if (!previous) {
		return FirstChild();
	} else {
		assert(previous->parent == this);
		return previous->NextSibling();
	}
}

// tinyxml.cpp:380
TiXmlNode* TiXmlNode::IterateChildren(const char* val, TiXmlNode* previous) const {
	if (!previous) {
		return FirstChild(val);
	} else {
		assert(previous->parent == this);
		return previous->NextSibling(val);
	}
}

// tinyxml.cpp:393
TiXmlNode* TiXmlNode::NextSibling(const char* value) const {
	TiXmlNode* node;
	for (node = next; node; node = node->next) {
		if (node->SValue() == TIXML_STRING(value))
			return node;
	}
	return 0;
}

// tinyxml.cpp:405
TiXmlNode* TiXmlNode::PreviousSibling(const char* value) const {
	TiXmlNode* node;
	for (node = prev; node; node = node->prev) {
		if (node->SValue() == TIXML_STRING(value))
			return node;
	}
	return 0;
}

// tinyxml.cpp:416
void TiXmlElement::RemoveAttribute(const char* name) {
	TiXmlAttribute* node = attributeSet.Find(name);
	if (node) {
		attributeSet.Remove(node);
		delete node;
	}
}

// tinyxml.cpp:426
TiXmlElement* TiXmlNode::FirstChildElement() const {
	TiXmlNode* node;

	for (node = FirstChild(); node; node = node->NextSibling()) {
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}

// tinyxml.cpp:440
TiXmlElement* TiXmlNode::FirstChildElement(const char* value) const {
	TiXmlNode* node;

	for (node = FirstChild(value); node; node = node->NextSibling(value)) {
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}

// tinyxml.cpp:455
TiXmlElement* TiXmlNode::NextSiblingElement() const {
	TiXmlNode* node;

	for (node = NextSibling(); node; node = node->NextSibling()) {
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}

// tinyxml.cpp:469
TiXmlElement* TiXmlNode::NextSiblingElement(const char* value) const {
	TiXmlNode* node;

	for (node = NextSibling(value); node; node = node->NextSibling(value)) {
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}

// tinyxml.cpp:485
TiXmlDocument* TiXmlNode::GetDocument() const {
	const TiXmlNode* node;

	for (node = this; node; node = node->parent) {
		if (node->ToDocument())
			return node->ToDocument();
	}
	return 0;
}

// tinyxml.cpp:499
TiXmlElement::TiXmlElement(const char* _value) : TiXmlNode(TiXmlNode::ELEMENT) {
	firstChild = lastChild = 0;
	value = _value;
}

// tinyxml.cpp:505
TiXmlElement::~TiXmlElement() {
	while (attributeSet.First()) {
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove(node);
		delete node;
	}
}

// tinyxml.cpp:515
std::string TiXmlElement::Attribute(const char* name) const {
	TiXmlAttribute* node = attributeSet.Find(name);

	if (node)
		return node->Value();

	return "";
}

// tinyxml.cpp:525
const char* TiXmlElement::Attribute(const char* name, int* i) const {
	TiXmlAttribute* node = attributeSet.Find(name);
	if (node) {
		const char* s = node->Value();
		if (i) {
			if (s)
				*i = atoi(s);
			else
				*i = 0;
		}
		return s;
	}

	if (i)
		*i = 0;

	return 0;
}

// tinyxml.cpp:551
void TiXmlElement::SetAttribute(const char* name, int val) {
	char buf[64];
	sprintf(buf, "%d", val);
	SetAttribute(name, buf);
}

// tinyxml.cpp:559
void TiXmlElement::SetAttribute(const char* name, const char* value) {
	TiXmlAttribute* node = attributeSet.Find(name);
	if (node) {
		node->SetValue(value);
		return;
	}

	TiXmlAttribute* attrib = new TiXmlAttribute(name, value);
	if (attrib) {
		attributeSet.Add(attrib);
	} else {
		TiXmlDocument* document = GetDocument();
		if (document)
			document->SetError(TIXML_ERROR_OUT_OF_MEMORY);
	}
}

// tinyxml.cpp:580
void TiXmlElement::Print(FILE* cfile, int depth) const {
	int i;
	for (i = 0; i < depth; i++) {
		fprintf(cfile, "    ");
	}
	fprintf(cfile, "<%s", value.c_str());

	TiXmlAttribute* attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next()) {
		fprintf(cfile, " ");
		attrib->Print(cfile, depth);
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode* node;
	if (!firstChild) {
		fprintf(cfile, " />");
	} else if (firstChild == lastChild && firstChild->ToText()) {
		fprintf(cfile, ">");
		firstChild->Print(cfile, depth + 1);
		fprintf(cfile, "</%s>", value.c_str());
	} else {
		fprintf(cfile, ">");

		for (node = firstChild; node; node = node->NextSibling()) {
			if (!node->ToText()) {
				fprintf(cfile, "\n");
			}
			node->Print(cfile, depth + 1);
		}
		fprintf(cfile, "\n");
		for (i = 0; i < depth; ++i)
			fprintf(cfile, "    ");
		fprintf(cfile, "</%s>", value.c_str());
	}
}

// tinyxml.cpp:631
void TiXmlElement::StreamOut(TIXML_OSTREAM* stream) const {
	(*stream) << "<" << value;

	TiXmlAttribute* attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next()) {
		(*stream) << " ";
		attrib->StreamOut(stream);
	}

	// If this node has children, give it a closing tag. Else
	// make it an empty tag.
	TiXmlNode* node;
	if (firstChild) {
		(*stream) << ">";

		for (node = firstChild; node; node = node->NextSibling()) {
			node->StreamOut(stream);
		}
		(*stream) << "</" << value << ">";
	} else {
		(*stream) << " />";
	}
}

// tinyxml.cpp:661
TiXmlNode* TiXmlElement::Clone() const {
	TiXmlElement* clone = new TiXmlElement(Value());
	if (!clone)
		return 0;

	CopyToClone(clone);

	// Clone the attributes, then clone the children.
	TiXmlAttribute* attribute = 0;
	for (attribute = attributeSet.First();
		attribute;
		attribute = attribute->Next()) {
		clone->SetAttribute(attribute->Name(), attribute->Value());
	}

	TiXmlNode* node = 0;
	for (node = firstChild; node; node = node->NextSibling()) {
		clone->LinkEndChild(node->Clone());
	}
	return clone;
}

// tinyxml.cpp:687
TiXmlDocument::TiXmlDocument() : TiXmlNode(TiXmlNode::DOCUMENT) {
	error = false;
}

// tinyxml.cpp:693
TiXmlDocument::TiXmlDocument(const char* documentName) : TiXmlNode(TiXmlNode::DOCUMENT) {
	value = documentName;
	error = false;
}

// tinyxml.cpp:701
bool TiXmlDocument::LoadFile() {
	setlocale(LC_ALL, "Kor");

	StringToBuffer buf(value);

	if (buf.buffer && LoadFile(buf.buffer))
		return true;

	return false;
}

// tinyxml.cpp:727
bool TiXmlDocument::SaveFile() const {
	StringToBuffer buf(value);

	if (buf.buffer && SaveFile(buf.buffer))
		return true;

	return false;
}

// tinyxml.cpp:738
bool TiXmlDocument::LoadFile(const char* filename) {
	// Delete the existing data:
	Clear();

	// Assigning straight from filename (StringToBuffer works around the
	// STL string bug in the callers).
	value = filename;

	FILE* file = fopen(value.c_str(), "r");

	if (file) {
		// Get the file size, so we can pre-allocate the string.
		long length = 0;
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);

		// Strange case, but good to handle up front.
		if (length == 0) {
			fclose(file);
			return false;
		}

		// Assume it is all one big XML file, and read it in.
		TIXML_STRING data;
		data.reserve(length);

		const int BUF_SIZE = 2048;
		char buf[BUF_SIZE];

		while (fgets(buf, BUF_SIZE, file)) {
			data += buf;
		}
		fclose(file);

		Parse(data.c_str());
		if (!Error()) {
			return true;
		}
	}
	SetError(TIXML_ERROR_OPENING_FILE);
	return false;
}

// tinyxml.cpp:797
bool TiXmlDocument::LoadFileEx(const char* filename) {
	cFile* file = g_resrcmng->GetCFile(filename, 0xffff);
	if (file) {
		char* buffer = new char[file->Length() + 1];
		file->Read(buffer, file->Length());
		buffer[file->Length()] = 0;
		CloseCFile(file);

		Clear();
		Parse(buffer);

		delete[] buffer;

		return Error() ? false : true;
	}
	SetError(TIXML_ERROR_OPENING_FILE);
	return false;
}

// tinyxml.cpp:823
bool TiXmlDocument::LoadFileFromMemoryEx(const char* data) {
	Clear();
	Parse(data);

	return Error() ? false : true;
}

// tinyxml.cpp:831
bool TiXmlDocument::SaveFile(const char* filename) const {
	// The old c stuff lives on...
	FILE* fp = fopen(filename, "w");
	if (fp) {
		Print(fp, 0);
		fclose(fp);
		return true;
	}
	return false;
}

// tinyxml.cpp:845
TiXmlNode* TiXmlDocument::Clone() const {
	TiXmlDocument* clone = new TiXmlDocument();
	if (!clone)
		return 0;

	CopyToClone(clone);
	clone->error = error;
	clone->errorDesc = errorDesc.c_str();

	TiXmlNode* node = 0;
	for (node = firstChild; node; node = node->NextSibling()) {
		clone->LinkEndChild(node->Clone());
	}
	return clone;
}

// tinyxml.cpp:864
void TiXmlDocument::Print(FILE* cfile, int depth) const {
	TiXmlNode* node;
	for (node = FirstChild(); node; node = node->NextSibling()) {
		node->Print(cfile, depth);
		fprintf(cfile, "\n");
	}
}

// tinyxml.cpp:874
void TiXmlDocument::StreamOut(TIXML_OSTREAM* out) const {
	TiXmlNode* node;
	for (node = FirstChild(); node; node = node->NextSibling()) {
		node->StreamOut(out);

		// Special rule for streams: stop after the root element.
		// The stream in code will only read one element, so don't
		// write more than one.
		if (node->ToElement())
			break;
	}
}

// tinyxml.cpp:889
TiXmlAttribute* TiXmlAttribute::Next() const {
	// The sentinel has neither a name nor a value.
	if (next->value.empty() && next->name.empty())
		return 0;
	return next;
}

// tinyxml.cpp:899
TiXmlAttribute* TiXmlAttribute::Previous() const {
	// The sentinel has neither a name nor a value.
	if (prev->value.empty() && prev->name.empty())
		return 0;
	return prev;
}

// tinyxml.cpp:909
void TiXmlAttribute::Print(FILE* cfile, int depth) const {
	TIXML_STRING n, v;

	PutString(Name(), &n);
	PutString(Value(), &v);

	if (value.find('\"') == TIXML_STRING::npos)
		fprintf(cfile, "%s=\"%s\"", n.c_str(), v.c_str());
	else
		fprintf(cfile, "%s='%s'", n.c_str(), v.c_str());
}

// tinyxml.cpp:923
void TiXmlAttribute::StreamOut(TIXML_OSTREAM* stream) const {
	if (value.find('\"') != TIXML_STRING::npos) {
		PutString(name, stream);
		(*stream) << "=" << "'";
		PutString(value, stream);
		(*stream) << "'";
	} else {
		PutString(name, stream);
		(*stream) << "=" << "\"";
		PutString(value, stream);
		(*stream) << "\"";
	}
}

// tinyxml.cpp:941
void TiXmlAttribute::SetIntValue(int value) {
	char buf[64];
	sprintf(buf, "%d", value);
	SetValue(buf);
}

// tinyxml.cpp:948
void TiXmlAttribute::SetDoubleValue(double value) {
	char buf[64];
	sprintf(buf, "%lf", value);
	SetValue(buf);
}

// tinyxml.cpp:955
const int TiXmlAttribute::IntValue() const {
	return atoi(value.c_str());
}

// tinyxml.cpp:960
const double TiXmlAttribute::DoubleValue() const {
	return atof(value.c_str());
}

// tinyxml.cpp:965
void TiXmlComment::Print(FILE* cfile, int depth) const {
	for (int i = 0; i < depth; i++) {
		fputs("    ", cfile);
	}
	fprintf(cfile, "<!--%s-->", value.c_str());
}

// tinyxml.cpp:974
void TiXmlComment::StreamOut(TIXML_OSTREAM* stream) const {
	(*stream) << "<!--";
	PutString(value, stream);
	(*stream) << "-->";
}

// tinyxml.cpp:981
TiXmlNode* TiXmlComment::Clone() const {
	TiXmlComment* clone = new TiXmlComment();

	if (!clone)
		return 0;

	CopyToClone(clone);
	return clone;
}

// tinyxml.cpp:993
void TiXmlText::Print(FILE* cfile, int depth) const {
	TIXML_STRING buffer;
	PutString(value, &buffer);
	fprintf(cfile, "%s", buffer.c_str());
}

// tinyxml.cpp:1001
void TiXmlText::StreamOut(TIXML_OSTREAM* stream) const {
	PutString(value, stream);
}

// tinyxml.cpp:1007
TiXmlNode* TiXmlText::Clone() const {
	TiXmlText* clone = 0;
	clone = new TiXmlText("");

	if (!clone)
		return 0;

	CopyToClone(clone);
	return clone;
}

// tinyxml.cpp:1023
TiXmlDeclaration::TiXmlDeclaration(const char* _version, const char* _encoding, const char* _standalone)
	: TiXmlNode(TiXmlNode::DECLARATION) {
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}

// tinyxml.cpp:1031
void TiXmlDeclaration::Print(FILE* cfile, int depth) const {
	fprintf(cfile, "<?xml ");

	if (!version.empty())
		fprintf(cfile, "version=\"%s\" ", version.c_str());
	if (!encoding.empty())
		fprintf(cfile, "encoding=\"%s\" ", encoding.c_str());
	if (!standalone.empty())
		fprintf(cfile, "standalone=\"%s\" ", standalone.c_str());
	fprintf(cfile, "?>");
}

// tinyxml.cpp:1044
void TiXmlDeclaration::StreamOut(TIXML_OSTREAM* stream) const {
	(*stream) << "<?xml ";

	if (!version.empty()) {
		(*stream) << "version=\"";
		PutString(version, stream);
		(*stream) << "\" ";
	}
	if (!encoding.empty()) {
		(*stream) << "encoding=\"";
		PutString(encoding, stream);
		(*stream) << "\" ";
	}
	if (!standalone.empty()) {
		(*stream) << "standalone=\"";
		PutString(standalone, stream);
		(*stream) << "\" ";
	}
	(*stream) << "?>";
}

// tinyxml.cpp:1069
TiXmlNode* TiXmlDeclaration::Clone() const {
	TiXmlDeclaration* clone = new TiXmlDeclaration();

	if (!clone)
		return 0;

	CopyToClone(clone);
	clone->version = version;
	clone->encoding = encoding;
	clone->standalone = standalone;
	return clone;
}

// tinyxml.cpp:1084
void TiXmlUnknown::Print(FILE* cfile, int depth) const {
	for (int i = 0; i < depth; i++)
		fprintf(cfile, "    ");
	fprintf(cfile, "%s", value.c_str());
}

// tinyxml.cpp:1091
void TiXmlUnknown::StreamOut(TIXML_OSTREAM* stream) const {
	(*stream) << "<" << value << ">";
}

// tinyxml.cpp:1096
TiXmlNode* TiXmlUnknown::Clone() const {
	TiXmlUnknown* clone = new TiXmlUnknown();

	if (!clone)
		return 0;

	CopyToClone(clone);
	return clone;
}

// tinyxml.cpp:1108
TiXmlAttributeSet::TiXmlAttributeSet() {
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}

// tinyxml.cpp:1115
TiXmlAttributeSet::~TiXmlAttributeSet() {
	assert(sentinel.next == &sentinel);
	assert(sentinel.prev == &sentinel);
}

// tinyxml.cpp:1122
void TiXmlAttributeSet::Add(TiXmlAttribute* addMe) {
	assert(!Find(addMe->Name()));

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev = addMe;
}

// tinyxml.cpp:1133
void TiXmlAttributeSet::Remove(TiXmlAttribute* removeMe) {
	TiXmlAttribute* node;

	for (node = sentinel.next; node != &sentinel; node = node->next) {
		if (node == removeMe) {
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert(0);
}

// tinyxml.cpp:1151
TiXmlAttribute* TiXmlAttributeSet::Find(const char* name) const {
	TiXmlAttribute* node;

	for (node = sentinel.next; node != &sentinel; node = node->next) {
		if (node->name == name)
			return node;
	}
	return 0;
}

// tinyxml.cpp:1177
TIXML_OSTREAM& operator<<(TIXML_OSTREAM& out, const TiXmlNode& base) {
	base.StreamOut(&out);
	return out;
}
