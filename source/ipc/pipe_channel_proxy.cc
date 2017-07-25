//
// PROJECT:         Aspia Remote Desktop
// FILE:            ipc/pipe_channel_proxy.cc
// LICENSE:         Mozilla Public License Version 2.0
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "ipc/pipe_channel_proxy.h"

namespace aspia {

PipeChannelProxy::PipeChannelProxy(PipeChannel* channel) :
    channel_(channel)
{
    // Nothing
}

void PipeChannelProxy::WillDestroyCurrentChannel()
{
    std::lock_guard<std::mutex> lock(channel_lock_);
    channel_ = nullptr;
    stop_event_.Signal();
}

bool PipeChannelProxy::Send(IOBuffer buffer,
                            PipeChannel::SendCompleteHandler handler)
{
    std::lock_guard<std::mutex> lock(channel_lock_);

    if (channel_)
    {
        channel_->Send(std::move(buffer), std::move(handler));
        return true;
    }

    return false;
}

bool PipeChannelProxy::Receive(PipeChannel::ReceiveCompleteHandler handler)
{
    std::lock_guard<std::mutex> lock(channel_lock_);

    if (channel_)
    {
        channel_->Receive(std::move(handler));
        return true;
    }

    return false;
}

void PipeChannelProxy::WaitForDisconnect()
{
    stop_event_.Wait();
}

} // namespace aspia
