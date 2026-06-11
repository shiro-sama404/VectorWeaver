#include "vectorweaver/vector_clock.hpp"
#include <algorithm>

namespace vw
{
void
VectorClock::update(const ClockState& received_clock)
{
    for (const auto& [id, value] : received_clock)
        if (clock.find(id) == clock.end())
            clock[id] = value;
        else
            clock[id] = std::max(clock[id], value);
}
} // namespace vw