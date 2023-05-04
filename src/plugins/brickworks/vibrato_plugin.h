/*
 * Copyright 2017-2023 Elk Audio AB
 *
 * SUSHI is free software: you can redistribute it and/or modify it under the terms of
 * the GNU Affero General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * SUSHI is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along with
 * SUSHI.  If not, see http://www.gnu.org/licenses/
 */

/**
 * @brief Vibrato from Brickworks library
 * @copyright 2017-2023 Elk Audio AB, Stockholm
 */

#ifndef VIBRATO_PLUGIN_H
#define VIBRATO_PLUGIN_H

#include <bw_chorus.h>

#include "library/internal_plugin.h"

namespace sushi {
namespace vibrato_plugin {

constexpr int MAX_CHANNELS_SUPPORTED = 8;

class VibratoPlugin : public InternalPlugin, public UidHelper<VibratoPlugin>
{
public:
    VibratoPlugin(HostControl hostControl);

    ~VibratoPlugin();

    virtual ProcessorReturnCode init(float sample_rate) override;

    void configure(float sample_rate) override;

    void set_enabled(bool enabled) override;

    void process_audio(const ChunkSampleBuffer &in_buffer, ChunkSampleBuffer &out_buffer) override;

    static std::string_view static_uid();

private:
    FloatParameterValue* _rate;
    FloatParameterValue* _amount;

    bw_chorus_coeffs _chorus_coeffs;
    std::array<bw_chorus_state, MAX_CHANNELS_SUPPORTED> _chorus_states;
    std::array<void*, MAX_CHANNELS_SUPPORTED> _delay_mem_areas{nullptr};
};

}// namespace vibrato_plugin
}// namespace sushi
#endif // VIBRATO_PLUGIN_H
