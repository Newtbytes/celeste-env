#pragma once

#define SCREEN_SIZE 128
#define SPR_SIZE 8


// #define SPRITE_LIST \
//     X(0)   X(1)   X(2)   X(3)   X(4)   X(5)   X(6)   X(7)   \
//     X(8)   X(9)   X(10)  X(11)  X(12)  X(13)  X(14)  X(15)  \
//     X(16)  X(17)  X(18)  X(19)  X(20)  X(21)  X(22)  X(23)  \
//     X(24)  X(25)  X(26)  X(27)  X(28)  X(29)  X(30)  X(31)  \
//     X(32)  X(33)  X(34)  X(35)  X(36)  X(37)  X(38)  X(39)  \
//     X(40)  X(41)  X(42)  X(43)  X(44)  X(45)  X(46)  X(47)  \
//     X(48)  X(49)  X(50)  X(51)  X(52)  X(53)  X(54)  X(55)  \
//     X(56)  X(57)  X(58)  X(59)  X(60)  X(61)  X(62)  X(63)  \
//     X(64)  X(65)  X(66)  X(67)  X(68)  X(69)  X(70)  X(71)  \
//     X(72)  X(73)  X(74)  X(75)  X(76)  X(77)  X(78)  X(79)  \
//     X(80)  X(81)  X(82)  X(83)  X(84)  X(85)  X(86)  X(87)  \
//     X(88)  X(89)  X(90)  X(91)  X(92)  X(93)  X(94)  X(95)  \
//     X(96)  X(97)  X(98)  X(99)  X(100) X(101) X(102) X(103) \
//     X(104) X(105) X(106) X(107) X(108) X(109) X(110) X(111) \
//     X(112) X(113) X(114) X(115) X(116) X(117) X(118) X(119) \
//     X(120) X(121) X(122) X(123) X(124) X(125) X(126) X(127)


const int palette[16] = {
    0x000000,
    0x1D2B53,
    0x7E2553,
    0x008751,
    0xAB5236,
    0x5F574F,
    0xC2C3C7,
    0xFFF1E8,
    0xFF004D,
    0xFFA300,
    0xFFEC27,
    0x00E436,
    0x29ADFF,
    0x83769C,
    0xFF77A8,
    0xFFCCAA
};

typedef void (*spr_renderer)(int x, int y);

void p8_init();
void p8_get_screen(unsigned char input_screen[SCREEN_SIZE][SCREEN_SIZE][3]);
void p8_spr(int spr, int x, int y, bool flipx, bool flipy);
void p8_print(const char* str, int x, int y, int col);
void p8_rectfill(int x0, int y0, int x1, int y1, int col);
