#include "synchronize.h"
namespace dark {

struct ModuleBase {
	virtual void work() = 0;
	virtual void sync() = 0;
	virtual ~ModuleBase() = default;
};

template<typename _Tinput, typename _Toutput, typename _Tprivate>
struct Module : public ModuleBase, public _Tinput, public _Toutput, protected _Tprivate {
	void sync() override final {
		sync_member(static_cast<_Tinput &>(*this));
		sync_member(static_cast<_Toutput &>(*this));
		sync_member(static_cast<_Tprivate &>(*this));
	}
};

}// namespace dark
