#pragma once

#include <vsg/vk/DeviceMemory.h>
#include <vsg/vk/CmdDraw.h>

namespace vsg
{
    class Buffer : public Object
    {
    public:
        Buffer(VkBuffer Buffer, VkBufferUsageFlags usage, VkSharingMode sharingMode, Device* device, AllocationCallbacks* allocator=nullptr);

        using Result = vsg::Result<Buffer, VkResult, VK_SUCCESS>;
        static Result create(Device* device, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, AllocationCallbacks* allocator=nullptr);

        VkBufferUsageFlags usage() const { return _usage; }
        VkSharingMode shaderMode() const { return _sharingMode; }
        VkBuffer buffer() const { return _buffer; }

        Device* getDevice() { return _device; }
        const Device* getDevice() const { return _device; }

        DeviceMemory* getDeviceMemory() { return _deviceMemory; }
        const DeviceMemory* getDeviceMemory() const { return _deviceMemory; }

        VkResult bind(DeviceMemory* deviceMemory, VkDeviceSize memoryOffset)
        {
            VkResult result = vkBindBufferMemory(*_device, _buffer, *deviceMemory, memoryOffset);
            if (result == VK_SUCCESS)
            {
                _deviceMemory = deviceMemory;
                _memoryOffset = memoryOffset;
            }
            return result;
        }

        operator VkBuffer () const { return _buffer; }

    protected:
        virtual ~Buffer();

        VkBuffer                        _buffer;
        VkBufferUsageFlags              _usage;
        VkSharingMode                   _sharingMode;


        ref_ptr<Device>                 _device;
        ref_ptr<AllocationCallbacks>    _allocator;

        ref_ptr<DeviceMemory>           _deviceMemory;
        VkDeviceSize                    _memoryOffset;
    };


    class CmdBindVertexBuffers : public Command
    {
    public:

        CmdBindVertexBuffers() : _firstBinding(0) {}

        virtual void accept(Visitor& visitor) { visitor.apply(*this); }

        void setFirstBinding(uint32_t firstBinding) { _firstBinding = firstBinding; }
        uint32_t getFirstBinding() const { return _firstBinding; }

        void add(Buffer* buffer, VkDeviceSize offset)
        {
            _buffers.push_back(buffer);
            _vkBuffers.push_back(*buffer);
            _offsets.push_back(offset);
        }

        virtual void dispatch(VkCommandBuffer commandBuffer) const
        {
            vkCmdBindVertexBuffers(commandBuffer, _firstBinding, _buffers.size(), _vkBuffers.data(), _offsets.data());
        }

    protected:
        virtual ~CmdBindVertexBuffers() {}

        using Buffers = std::vector<ref_ptr<Buffer>>;
        using VkBuffers = std::vector<VkBuffer>;
        using Offsets = std::vector<VkDeviceSize>;

        uint32_t _firstBinding;
        Buffers _buffers;
        VkBuffers _vkBuffers;
        Offsets _offsets;

    };


    class CmdBindIndexBuffer : public Command
    {
    public:

        CmdBindIndexBuffer(Buffer* buffer, VkDeviceSize offset, VkIndexType indexType) : _buffer(buffer), _offset(offset), _indexType(indexType) {}

        virtual void accept(Visitor& visitor) { visitor.apply(*this); }

        virtual void dispatch(VkCommandBuffer commandBuffer) const
        {
            vkCmdBindIndexBuffer(commandBuffer, *_buffer, _offset, _indexType);
        }

    protected:
        virtual ~CmdBindIndexBuffer() {}

        vsg::ref_ptr<Buffer> _buffer;
        VkDeviceSize _offset;
        VkIndexType _indexType;

    };
}
