#pragma once

class AnimUtils
{
public:
    template<typename T>
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if(!Animation) return nullptr;

        const auto NotyfyEvents = Animation -> Notifies;
        for(auto NotyfyEvent: NotyfyEvents)
        {
            auto AnimNotify = Cast<T>(NotyfyEvent.Notify);
            if(AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    };
};