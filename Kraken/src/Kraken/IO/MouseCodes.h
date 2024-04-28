//
// Created by sebsn on 28-04-2024.
//

#ifndef KR_MOUSECODES_H
#define KR_MOUSECODES_H

namespace Kraken::Mouse {
    using MouseCode = uint16_t;
    enum : MouseCode {
        Button0                = 0,
        Button1                = 1,
        Button2                = 2,
        Button3                = 3,
        Button4                = 4,
        Button5                = 5,
        Button6                = 6,
        Button7                = 7,

        ButtonLast             = Button7,
        ButtonLeft             = Button0,
        ButtonRight            = Button1,
        ButtonMiddle           = Button2
    };
}

#define KR_MOUSE_LAST 7

#endif //KR_MOUSECODES_H
