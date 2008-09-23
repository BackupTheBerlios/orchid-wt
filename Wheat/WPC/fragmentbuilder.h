#ifndef _ORCHID_FRAGMENTBUILDER_H_
#define _ORCHID_FRAGMENTBUILDER_H_

#include <flower/documentprocessor.h>

namespace Orchid {

class DomFragment;

class FragmentBuilderPrivate;
class FragmentBuilder : public DocumentProcessor {
public:
	FragmentBuilder();
	~FragmentBuilder();
public:
	void startDocument(const DocumentHead &head = DocumentHead());
	void endDocument();
	void startElement(Document::Tag tag);
	void endElement();
	void insertCharacters(const QString &str);
	void setAttribute(Document::Attribute attr, const QVariant &val);
public:
	DomFragment *fragment() const;
	DomFragment *takeFragment();
private:
	Q_DECLARE_PRIVATE(FragmentBuilder)
};

}

#endif
