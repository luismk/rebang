// Original: d:\build\custom\temp\code\client\projectg\gatewayinterface.h
#ifndef PG_CLIENT_PROJECTG_GATEWAYINTERFACE_H
#define PG_CLIENT_PROJECTG_GATEWAYINTERFACE_H

#include "..\..\defines.h"
#include "wtemplate.h"
#include "frcmdtarget.h"

class MsgObject;
struct IContentsDataContainer;

namespace _gateway {

// Handler of one gateway content: dispatches through its FunctionMapper and
// reads its document. Vtable from ??_7CGatewayHandler@_gateway@@6BFunctionMapper@@@
// (slots 1-2 pure).
class CGatewayHandler : public FunctionMapper, public FrCmdTarget {
public:
	// gatewayinterface.h:21
	CGatewayHandler(void) : m_pDocument(0) {}
	// gatewayinterface.h:22
	virtual ~CGatewayHandler(void) {}

	virtual bool Initialize(void) = 0;
	virtual bool Execute(const MsgObject& msg) = 0;

	// gatewayinterface.h:29
	void SetDocument(IContentsDataContainer* pDocument) { m_pDocument = pDocument; }

	template <class T>
	T GetDocument(void) { return (T)m_pDocument; }

	IContentsDataContainer* m_pDocument; // 0x18
};

CHECK_FIELD(CGatewayHandler, m_pDocument, 0x18);
CHECK_SIZE(CGatewayHandler, 0x1C);

}

#endif // PG_CLIENT_PROJECTG_GATEWAYINTERFACE_H
