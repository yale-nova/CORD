/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-NvidiaProprietary
 *
 * NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
 * property and proprietary rights in and to this material, related
 * documentation and any modifications thereto. Any use, reproduction,
 * disclosure or distribution of this material and related documentation
 * without an express license agreement from NVIDIA CORPORATION or
 * its affiliates is strictly prohibited.
 */


/*
 * Copyright (c) 2008 Princeton University
 * Copyright (c) 2016 Georgia Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Niket Agarwal
 *          Tushar Krishna
 */


#ifndef __MEM_RUBY_NETWORK_GARNET2_0_NETWORKLINK_HH__
#define __MEM_RUBY_NETWORK_GARNET2_0_NETWORKLINK_HH__

#include <iostream>
#include <vector>

#include "mem/ruby/common/Consumer.hh"
#include "mem/ruby/network/garnet2.0/CommonTypes.hh"
#include "mem/ruby/network/garnet2.0/flitBuffer.hh"
#include "params/NetworkLink.hh"
#include "sim/clocked_object.hh"

class GarnetNetwork;

class NetworkLink : public ClockedObject, public Consumer
{
  public:
    typedef NetworkLinkParams Params;
    NetworkLink(const Params *p);
    ~NetworkLink();

    void setLinkConsumer(Consumer *consumer);
    void setSourceQueue(flitBuffer *srcQueue);
    void setType(link_type type) { m_type = type; }
    link_type getType() { return m_type; }
    void print(std::ostream& out) const {}
    int get_id() const { return m_id; }
    void wakeup();

    unsigned int getLinkUtilization() const { return m_link_utilized; }
    const std::vector<unsigned int> & getVcLoad() const { return m_vc_load; }

    inline bool isReady(Cycles curTime)
    { return linkBuffer->isReady(curTime); }

    inline flit* peekLink()       { return linkBuffer->peekTopFlit(); }
    inline flit* consumeLink()    { return linkBuffer->getTopFlit(); }

    uint32_t functionalWrite(Packet *);
    bool functionalRead(Packet* pkt);

    void resetStats();
    // this is a hack, use 200 as a threshold to detect GC links. Need to be fixed later
    bool isCGLink() { return m_latency >= (Cycles)25; }
    uint64_t getLinkFlitBytes() { return m_link_flit_bytes; }
    uint64_t getLinkFlitCount() { return m_link_flit_count; }

  private:
    const int m_id;
    link_type m_type;
    const Cycles m_latency;

    flitBuffer *linkBuffer;
    Consumer *link_consumer;
    flitBuffer *link_srcQueue;

    // Statistical variables
    unsigned int m_link_utilized;
    std::vector<unsigned int> m_vc_load;

    // DO profiling
    uint64_t m_link_flit_bytes;
    uint64_t m_link_flit_count;
};

#endif // __MEM_RUBY_NETWORK_GARNET2_0_NETWORKLINK_HH__
