#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/vk/BufferView.h>
#include <vsg/vk/Descriptor.h>

namespace vsg
{
    using BufferViewList = std::vector<ref_ptr<BufferView>>;

    class VSG_DECLSPEC DescriptorTexelBufferView : public Inherit<Descriptor, DescriptorTexelBufferView>
    {
    public:
        DescriptorTexelBufferView(uint32_t dstBinding, uint32_t dstArrayElement, VkDescriptorType descriptorType, const BufferViewList& texelBufferViews);

        void assignTo(Context& context, VkWriteDescriptorSet& wds) const override;

        uint32_t getNumDescriptors() const override { return static_cast<uint32_t>(_texelBufferViewList.size()); }

    protected:
        BufferViewList _texelBufferViewList;
    };
    VSG_type_name(vsg::DescriptorTexelBufferView);

    struct material
    {
        vec4 ambientColor;
        vec4 diffuseColor;
        vec4 specularColor;
        float shininess;

        void read(vsg::Input& input)
        {
            input.read("ambientColor", ambientColor);
            input.read("diffuseColor", diffuseColor);
            input.read("specularColor", specularColor);
            input.read("shininess", shininess);
        }

        void write(vsg::Output& output) const
        {
            output.write("ambientColor", ambientColor);
            output.write("diffuseColor", diffuseColor);
            output.write("specularColor", specularColor);
            output.write("shininess", shininess);
        }
    };

    template<>
    constexpr bool has_read_write<material>() { return true; }

    VSG_value(materialValue, material);
    VSG_array(materialArray, material);

} // namespace vsg