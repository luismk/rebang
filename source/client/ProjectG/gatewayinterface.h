#pragma once

#include <map>
#include "actor.h"
#include "contentsdoc.h"
#include "wtemplate.h"

namespace _gateway
{
	class CGatewayHandler : public FunctionMapper, public FrCmdTarget
	{
	public:
		CGatewayHandler()
			: m_pDocument(NULL)
		{
		}
		virtual ~CGatewayHandler() { }

		virtual bool Initialize() = 0;
		virtual bool Execute(const MsgObject& msg) = 0;

		template <class T>
		T GetDocument()
		{
			return (T)m_pDocument;
		}
		void SetDocument(IContentsDataContainer* pDocument)
		{
			m_pDocument = pDocument;
		}

	protected:
		IContentsDataContainer* m_pDocument;
	};
}
