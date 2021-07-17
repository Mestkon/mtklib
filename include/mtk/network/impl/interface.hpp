#ifndef MTK_NETWORK_IMPL_INTERFACE_HPP
#define MTK_NETWORK_IMPL_INTERFACE_HPP

#include <mtk/core/types.hpp>

#include <string>
#include <list>

namespace mtk {

extern const int _iff_up;
extern const int _iff_broadcast;
extern const int _iff_loopback;
extern const int _iff_p2p;
extern const int _iff_running;
extern const int _iff_multicast;

struct _interface_entry
{
	std::string name;
	size_t index;
	uint32_t flags;
	int af;
	uint32_t flow;
	uint32_t scope;
	uint32_t address[4];
};

std::list<_interface_entry>
_get_network_interfaces(bool& err);

} // namespace mtk

#endif
