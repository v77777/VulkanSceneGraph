/* <editor-fold desc="MIT License">

Copyright(c) 2024 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/animation/AnimationManager.h>
#include <vsg/io/Options.h>

using namespace vsg;

AnimationManager::AnimationManager()
{
}

void AnimationManager::assignInstrumentation(ref_ptr<Instrumentation> in_instrumentation)
{
    instrumentation = in_instrumentation;
}

bool AnimationManager::start(vsg::ref_ptr<vsg::Animation> animation)
{
    CPU_INSTRUMENTATION_L1_NC(instrumentation, "AnimationManager start Animation", COLOR_VIEWER);

    if (std::find(animations.begin(), animations.end(), animation) != animations.end())
    {
        return false;
    }

    animation->startTime = _simulationTime;
    animations.push_back(animation);

    return true;
}

bool AnimationManager::end(vsg::ref_ptr<vsg::Animation> animation)
{
    CPU_INSTRUMENTATION_L1_NC(instrumentation, "AnimationManager end Animation", COLOR_VIEWER);

    auto itr = std::find(animations.begin(), animations.end(), animation);
    if (itr != animations.end())
    {
        animations.erase(itr);
        return true;
    }
    else
    {
        return false;
    }
}

void AnimationManager::run(vsg::ref_ptr<vsg::FrameStamp> frameStamp)
{
    CPU_INSTRUMENTATION_L1_NC(instrumentation, "AnimationManager run", COLOR_VIEWER);

    _simulationTime = frameStamp->simulationTime;

    for (auto& animation : animations)
    {
        animation->update(_simulationTime);
    }
}
