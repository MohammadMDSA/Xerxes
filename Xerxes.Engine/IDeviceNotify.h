#pragma once

namespace Xerxes
{
    namespace Engine
    {
        namespace Graphics
        {
            namespace Device
            {
                // Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
                interface IDeviceNotify
                {
                    virtual void OnDeviceLost() = 0;
                    virtual void OnDeviceRestored() = 0;

                protected:
                    ~IDeviceNotify() = default;
                };
            }
        }
    }
}