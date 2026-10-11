// Original: d:\build\custom\temp\code\client\projectg\tinyxmlparser.cpp
// TinyXML 2.0-era parser (Lee Thomason), compiled into ProjectG without TIXML_USE_STL.
#include "tinyxml.h"

#include <ctype.h>

TiXmlBase::Entity TiXmlBase::entity[NUM_ENTITY] = {
	{ "&amp;", 5, '&' },
	{ "&lt;", 4, '<' },
	{ "&gt;", 4, '>' },
	{ "&quot;", 6, '\"' },
	{ "&apos;", 6, '\'' }
};

// tinyxmlparser.cpp:58
const char* TiXmlBase::SkipWhiteSpace(const char* p) {
	if (!p || !*p) {
		return 0;
	}
	while (p && *p) {
		if (isspace((unsigned char)*p) || *p == '\n' || *p == '\r')
			++p;
		else
			break;
	}

	return p;
}

// tinyxmlparser.cpp:104
const char* TiXmlBase::ReadName(const char* p, TIXML_STRING* name) {
	*name = "";
	assert(p);

	// Names start with letters or underscores. After that, they can be
	// letters, underscores, numbers, hyphens, or colons.
	if (p && *p && (isalpha((unsigned char)*p) || *p == '_')) {
		while (p && *p && (isalnum((unsigned char)*p) || *p == '_' || *p == '-' || *p == ':')) {
			(*name) += *p;
			++p;
		}
		return p;
	}
	return 0;
}

// tinyxmlparser.cpp:130
const char* TiXmlBase::GetEntity(const char* p, char* value) {
	int i;

	// Ignore the &#x entities.
	if (strncmp("&#x", p, 3) == 0 && *(p + 3) && *(p + 4)) {
		*value = 0;

		if (isalpha(*(p + 3)))
			*value += (tolower(*(p + 3)) - 'a' + 10) * 16;
		else
			*value += (*(p + 3) - '0') * 16;

		if (isalpha(*(p + 4)))
			*value += (tolower(*(p + 4)) - 'a' + 10);
		else
			*value += (*(p + 4) - '0');

		return p + 6;
	}

	// Now try to match it.
	for (i = 0; i < NUM_ENTITY; ++i) {
		if (strncmp(entity[i].str, p, entity[i].strLength) == 0) {
			assert(strlen(entity[i].str) == entity[i].strLength);
			*value = entity[i].chr;
			return (p + entity[i].strLength);
		}
	}

	// So it wasn't an entity, its unrecognized, or something like that.
	*value = *p;
	return p + 1;
}

// tinyxmlparser.cpp:171
bool TiXmlBase::StringEqual(const char* p, const char* tag, bool ignoreCase) {
	assert(p);
	if (!p || !*p) {
		assert(0);
		return false;
	}

	if (tolower(*p) == tolower(*tag)) {
		const char* q = p;

		if (ignoreCase) {
			while (*q && *tag && *q == *tag) {
				++q;
				++tag;
			}

			if (*tag == 0) {
				return true;
			}
		} else {
			while (*q && *tag && tolower(*q) == tolower(*tag)) {
				++q;
				++tag;
			}

			if (*tag == 0) {
				return true;
			}
		}
	}
	return false;
}

// tinyxmlparser.cpp:225
const char* TiXmlBase::ReadText(const char* p, TIXML_STRING* text, bool trimWhiteSpace, const char* endTag, bool caseInsensitive) {
	*text = "";
	if (!trimWhiteSpace || !condenseWhiteSpace) {
		// Keep all the white space.
		while (p && *p && !StringEqual(p, endTag, caseInsensitive)) {
			char c;
			p = GetChar(p, &c);
			(*text) += c;
		}
	} else {
		bool whitespace = false;

		// Remove leading white space:
		p = SkipWhiteSpace(p);
		while (p && *p && !StringEqual(p, endTag, caseInsensitive)) {
			if (*p == '\r' || *p == '\n') {
				whitespace = true;
				++p;
			} else if (isspace((unsigned char)*p)) {
				whitespace = true;
				++p;
			} else {
				// If we've found whitespace, add it before the new character.
				// Any whitespace just becomes a space.
				if (whitespace) {
					(*text) += ' ';
					whitespace = false;
				}
				char c;
				p = GetChar(p, &c);
				(*text) += c;
			}
		}
	}
	return p + strlen(endTag);
}

// tinyxmlparser.cpp:338
const char* TiXmlDocument::Parse(const char* p) {
	// Parse away, at the document level. Since a document contains nothing
	// but other tags, most of what happens here is skipping white space.
	if (!p || !*p) {
		SetError(TIXML_ERROR_DOCUMENT_EMPTY);
		return 0;
	}

	p = SkipWhiteSpace(p);
	if (!p) {
		SetError(TIXML_ERROR_DOCUMENT_EMPTY);
		return 0;
	}

	while (p && *p) {
		TiXmlNode* node = Identify(p);
		if (node) {
			p = node->Parse(p);
			LinkEndChild(node);
		} else {
			break;
		}
		p = SkipWhiteSpace(p);
	}
	// All is well.
	return p;
}

// tinyxmlparser.cpp:380
TiXmlNode* TiXmlNode::Identify(const char* p) {
	TiXmlNode* returnNode = 0;

	p = SkipWhiteSpace(p);
	if (!p || !*p || *p != '<') {
		return 0;
	}

	TiXmlDocument* doc = GetDocument();
	p = SkipWhiteSpace(p);

	if (!p || !*p) {
		return 0;
	}

	// What is this thing?
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Declaration: <?xml
	// - Everything else is unknown to tinyxml.
	const char* xmlHeader = { "<?xml" };
	const char* commentHeader = { "<!--" };

	if (StringEqual(p, xmlHeader, true)) {
		returnNode = new TiXmlDeclaration();
	} else if (isalpha(*(p + 1)) || *(p + 1) == '_') {
		returnNode = new TiXmlElement("");
	} else if (StringEqual(p, commentHeader, false)) {
		returnNode = new TiXmlComment();
	} else {
		returnNode = new TiXmlUnknown();
	}

	if (returnNode) {
		// Set the parent, so it can report errors
		returnNode->parent = this;
	} else {
		if (doc)
			doc->SetError(TIXML_ERROR_OUT_OF_MEMORY);
	}
	return returnNode;
}

// tinyxmlparser.cpp:558
const char* TiXmlElement::Parse(const char* p) {
	p = SkipWhiteSpace(p);
	TiXmlDocument* document = GetDocument();

	if (!p || !*p || *p != '<') {
		if (document)
			document->SetError(TIXML_ERROR_PARSING_ELEMENT);
		return 0;
	}

	p = SkipWhiteSpace(p + 1);

	// Read the name.
	p = ReadName(p, &value);
	if (!p || !*p) {
		if (document)
			document->SetError(TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME);
		return 0;
	}

	TIXML_STRING endTag("</");
	endTag += value;
	endTag += ">";

	// Check for and read attributes. Also look for an empty tag or an end tag.
	while (p && *p) {
		p = SkipWhiteSpace(p);
		if (!p || !*p) {
			if (document)
				document->SetError(TIXML_ERROR_READING_ATTRIBUTES);
			return 0;
		}
		if (*p == '/') {
			++p;
			// Empty tag.
			if (*p != '>') {
				if (document)
					document->SetError(TIXML_ERROR_PARSING_EMPTY);
				return 0;
			}
			return (p + 1);
		} else if (*p == '>') {
			// Done with attributes (if there were any.) Read the value -- which
			// can include other elements -- read the end tag, and return.
			++p;
			p = ReadValue(p);
			if (!p || !*p)
				return 0;

			// We should find the end tag now
			if (StringEqual(p, endTag.c_str(), false)) {
				p += endTag.length();
				return p;
			} else {
				if (document)
					document->SetError(TIXML_ERROR_READING_END_TAG);
				return 0;
			}
		} else {
			// Try to read an attribute:
			TiXmlAttribute attrib;
			attrib.SetDocument(document);
			p = attrib.Parse(p);

			if (!p || !*p) {
				if (document)
					document->SetError(TIXML_ERROR_PARSING_ELEMENT);
				return 0;
			}
			SetAttribute(attrib.Name(), attrib.Value());
		}
	}
	return p;
}

// tinyxmlparser.cpp:645
const char* TiXmlElement::ReadValue(const char* p) {
	TiXmlDocument* document = GetDocument();

	// Read in text and elements in any order.
	p = SkipWhiteSpace(p);
	while (p && *p) {
		if (*p != '<') {
			// Take what we have, make a text element.
			TiXmlText* textNode = new TiXmlText("");

			if (!textNode) {
				if (document)
					document->SetError(TIXML_ERROR_OUT_OF_MEMORY);
				return 0;
			}

			p = textNode->Parse(p);

			if (!textNode->Blank())
				LinkEndChild(textNode);
			else
				delete textNode;
		} else {
			// We hit a '<'. Have we hit a new element or an end tag?
			if (StringEqual(p, "</", false)) {
				return p;
			} else {
				TiXmlNode* node = Identify(p);
				if (node) {
					p = node->Parse(p);
					LinkEndChild(node);
				} else {
					return 0;
				}
			}
		}
		p = SkipWhiteSpace(p);
	}

	if (!p) {
		if (document)
			document->SetError(TIXML_ERROR_READING_ELEMENT_VALUE);
	}
	return p;
}

// tinyxmlparser.cpp:722
const char* TiXmlUnknown::Parse(const char* p) {
	TiXmlDocument* document = GetDocument();
	p = SkipWhiteSpace(p);
	if (!p || !*p) {
		if (document)
			document->SetError(TIXML_ERROR_PARSING_UNKNOWN);
		return 0;
	}
	value = "";

	// Unset when p starts with neither "<![CDATA[" nor '<': the original reads it anyway.
	bool readToTagEnd;

	if (strnicmp(p, "<![CDATA[", 9) == 0) {
		p += 9;
		readToTagEnd = false;
	} else if (*p == '<') {
		++p;
		readToTagEnd = true;
	}

	if (!readToTagEnd) {
		while (p && *p && strnicmp(p, "]]>", 3) != 0) {
			value += *p;
			++p;
		}
		p += 3;
	} else {
		while (p && *p && *p != '>') {
			value += *p;
			++p;
		}
	}

	if (!p) {
		if (document)
			document->SetError(TIXML_ERROR_PARSING_UNKNOWN);
	}
	if (*p == '>')
		return p + 1;
	return p;
}

// tinyxmlparser.cpp:794
const char* TiXmlComment::Parse(const char* p) {
	TiXmlDocument* document = GetDocument();
	value = "";

	p = SkipWhiteSpace(p);
	const char* startTag = "<!--";
	const char* endTag = "-->";

	if (!StringEqual(p, startTag, false)) {
		document->SetError(TIXML_ERROR_PARSING_COMMENT);
		return 0;
	}
	p += strlen(startTag);
	p = ReadText(p, &value, false, endTag, false);
	return p;
}

// tinyxmlparser.cpp:814
const char* TiXmlAttribute::Parse(const char* p) {
	p = SkipWhiteSpace(p);
	if (!p || !*p)
		return 0;

	// Read the name, the '=' and the value.
	p = ReadName(p, &name);
	if (!p || !*p) {
		if (document)
			document->SetError(TIXML_ERROR_READING_ATTRIBUTES);
		return 0;
	}
	p = SkipWhiteSpace(p);
	if (!p || !*p || *p != '=') {
		if (document)
			document->SetError(TIXML_ERROR_READING_ATTRIBUTES);
		return 0;
	}

	++p; // skip '='
	p = SkipWhiteSpace(p);
	if (!p || !*p) {
		if (document)
			document->SetError(TIXML_ERROR_READING_ATTRIBUTES);
		return 0;
	}

	const char* end;

	if (*p == '\'') {
		++p;
		end = "\'";
		p = ReadText(p, &value, false, end, false);
	} else if (*p == '"') {
		++p;
		end = "\"";
		p = ReadText(p, &value, false, end, false);
	} else {
		// All attribute values should be in single or double quotes. But this
		// is such a common error that the parser will try its best, even
		// without them.
		value = "";
		while (p && *p && !isspace((unsigned char)*p) && *p != '\n' && *p != '\r' && *p != '/' && *p != '>') {
			value += *p;
			++p;
		}
	}
	return p;
}

// tinyxmlparser.cpp:887
const char* TiXmlText::Parse(const char* p) {
	value = "";

	bool ignoreWhite = true;
	const char* end = "<";
	p = ReadText(p, &value, ignoreWhite, end, false);
	if (p)
		return p - 1; // don't truncate the '<'
	return 0;
}

// tinyxmlparser.cpp:919
const char* TiXmlDeclaration::Parse(const char* p) {
	p = SkipWhiteSpace(p);
	// Find the beginning, find the end, and look for the stuff in-between.
	TiXmlDocument* document = GetDocument();
	if (!p || !*p || !StringEqual(p, "<?xml", true)) {
		if (document)
			document->SetError(TIXML_ERROR_PARSING_DECLARATION);
		return 0;
	}

	p += 5;

	version = "";
	encoding = "";
	standalone = "";

	while (p && *p) {
		if (*p == '>') {
			++p;
			return p;
		}

		p = SkipWhiteSpace(p);
		if (StringEqual(p, "version", true)) {
			TiXmlAttribute attrib;
			p = attrib.Parse(p);
			version = attrib.Value();
		} else if (StringEqual(p, "encoding", true)) {
			TiXmlAttribute attrib;
			p = attrib.Parse(p);
			encoding = attrib.Value();
		} else if (StringEqual(p, "standalone", true)) {
			TiXmlAttribute attrib;
			p = attrib.Parse(p);
			standalone = attrib.Value();
		} else {
			// Read over whatever it is.
			while (p && *p && *p != '>' && !isspace((unsigned char)*p))
				++p;
		}
	}
	return 0;
}

// tinyxmlparser.cpp:979
bool TiXmlText::Blank() const {
	for (unsigned i = 0; i < value.length(); i++)
		if (!isspace((unsigned char)value[i]))
			return false;
	return true;
}

