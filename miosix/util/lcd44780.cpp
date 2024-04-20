
#include <stdio.h>
#include <stdarg.h>
#include "miosix.h"
#include "interfaces/delays.h"
#include "lcd44780.h"

/// Fills data RAM with spaces, sets cursor address to 0, resets scrolling,
/// enables increment mode.
#define CMD_CLEAR                   (0x01)
/// Sets cursor address to 0 and resets scrolling
#define CMD_HOME                    (0x02)
/// Configures display update behaviour at every data byte sent.
/// @param dir   0: decrement data RAM address at every byte sent
///              1: increment data RAM address at every byte sent 
/// @param shift 0: sending data does not affect scrolling
///              1: sending data scrolls the display left/right depending on
///                 the data RAM address increment/decrement mode
#define CMD_ENTRY_MODE(dir,shift)   (0x04|((dir)<<1)|(shift))
/// Enables and disables display layers
/// @param disp  0: turns off the display, 1: turns on the display
/// @param cur   Cursor mode (see Cursor enum)
#define CMD_ENABLE(disp,cur)        (0x08|((disp)<<2)|(cur))
/// Move the cursor or scroll the display by 1 character forwards or backwards
/// @param shift 0: moves the cursor, 1: scrolls the display
/// @param rl    0: left (increment), 1: right (decrement)
#define CMD_SHIFT(shift,dir)        (0x10|((sc)<<3)|((rl)<<2))
/// Configures the controller for the display
/// @param dl    Data length
///              0: 4 bit, 1: 8 bit
/// @param lines Number of display lines
///              0: 1 line, 1: 2 lines
/// @param font  Size of the font (only for 1 line displays, otherwise ignored)
///              0: 5x8m 1: 5x10
#define CMD_FUNC_SET(dl,lines,font) (0x20|((dl)<<4)|((lines)<<3)|((font)<<2))
/// Sets the current write address in the font data RAM
#define CMD_CG_ADDR_SET(addr)       (0x40|(addr))
/// Sets the current write address in the display data RAM
#define CMD_DD_ADDR_SET(addr)       (0x80|(addr))

namespace miosix {

Lcd44780::Lcd44780(miosix::GpioPin rs, miosix::GpioPin e, miosix::GpioPin d4,
             miosix::GpioPin d5, miosix::GpioPin d6, miosix::GpioPin d7,
             int row, int col) : rs(rs), e(e), d4(d4), d5(d5), d6(d6), d7(d7),
             row(row), col(col)
{
    rs.mode(Mode::OUTPUT);
    e.mode(Mode::OUTPUT);
    d4.mode(Mode::OUTPUT);
    d5.mode(Mode::OUTPUT);
    d6.mode(Mode::OUTPUT);
    d7.mode(Mode::OUTPUT);
    e.low();
    Thread::sleep(50); //Powerup delay
    init();
    clear();
}

void Lcd44780::go(int x, int y)
{
    if(x<0 || x>=col || y<0 || y>=row) return;

    // 4x20 is implemented as 2x40.
    if(y>1) x += col;

    comd(CMD_DD_ADDR_SET(((y & 1) ? 0x40 : 0) | x)); //Move cursor
}

int Lcd44780::iprintf(const char* fmt, ...)
{
    va_list arg;
    char line[40];
    va_start(arg,fmt);
    int len=vsniprintf(line,sizeof(line),fmt,arg);
    va_end(arg);
    for(int i=0;i<len;i++) data(line[i]);
    return len;
}

int Lcd44780::printf(const char* fmt, ...)
{
    va_list arg;
    char line[40];
    va_start(arg,fmt);
    int len=vsnprintf(line,sizeof(line),fmt,arg);
    va_end(arg);
    for(int i=0;i<len;i++) data(line[i]);
    return len;
}

void Lcd44780::clear()
{
    comd(CMD_CLEAR);
    Thread::sleep(2); //Some displays require this delay
}

void Lcd44780::off()
{
    comd(CMD_ENABLE(0,0));
}

void Lcd44780::on()
{
    comd(CMD_ENABLE(1,cursorState));
}

void Lcd44780::cursor(Cursor cursor)
{
    cursorState=cursor & 0x3;
    comd(CMD_ENABLE(1,cursorState));
}

void Lcd44780::setCgram(int charCode, const unsigned char font[8])
{
    comd(CMD_CG_ADDR_SET((charCode & 0x7)<<3));
    for(int i=0;i<8;i++) data(font[i]);
    comd(CMD_DD_ADDR_SET(0));
}

void Lcd44780::init()
{
    rs.low();
    half(CMD_FUNC_SET(0,0,0));
    rs.high();
    delayUs(50);
    if(row==1) comd(CMD_FUNC_SET(0,0,0)); else comd(CMD_FUNC_SET(0,1,0));
    Thread::sleep(5);  //Initialization delay
    comd(CMD_ENABLE(1,cursorState)); //Display ON, cursor OFF, blink OFF
    comd(CMD_ENTRY_MODE(1,0));  //Auto increment ON, shift OFF
}

void Lcd44780::half(unsigned char byte)
{
    if(byte & (1<<7)) d7.high(); else d7.low(); //Slow but it works
    if(byte & (1<<6)) d6.high(); else d6.low();
    if(byte & (1<<5)) d5.high(); else d5.low();
    if(byte & (1<<4)) d4.high(); else d4.low();
    delayUs(1);
    e.high();
    delayUs(1);
    e.low();
}

void Lcd44780::data(unsigned char byte)
{
    half(byte);
    byte<<=4;
    half(byte);
    delayUs(50);
}

void Lcd44780::comd(unsigned char byte)
{
    rs.low();
    half(byte);
    byte<<=4;
    half(byte);
    delayUs(50);
    rs.high();
}

} //namespace miosix
