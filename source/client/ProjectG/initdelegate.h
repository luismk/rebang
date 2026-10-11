#ifndef PG_CLIENT_PROJECTG_INITDELEGATE_H
#define PG_CLIENT_PROJECTG_INITDELEGATE_H

// Registry of the functions run at start-up and at shut-down.
class CInitDelegate {
public:
	typedef void (*DelegateFunction)();

	enum {
		MAX_FUNCTIONS = 256
	};

	static void AddInitFunction(DelegateFunction pFunction);
	static void AddReleaseFunction(DelegateFunction pFunction);
	static void ExecuteInitFunctions();
	static void ExecuteReleaseFunctions();

private:
	static DelegateFunction m_InitFunctions[MAX_FUNCTIONS];
	static DelegateFunction m_ReleaseFunctions[MAX_FUNCTIONS];
	static int m_numInitFunctions;
	static int m_numReleaseFunctions;
};

#endif // PG_CLIENT_PROJECTG_INITDELEGATE_H
