/*
 * first kernel! woohoo! 
 *
 * original computer emulator 
 * built arpund early 2025 in roblox
 * 2026 - 07 - 22
 * Close to my birthday
*/

#include <stdint.h>
#include <stdarg.h>

#define UART_DATA ((volatile unsigned int*)(0x09000000))
#define UART_FR   ((volatile unsigned int*)(0x09000018))
#define HALTMSG "[sys] halt"

static char INPUT_CACHE[10][64]; 
static int  CACHE_INDEX = 0;      

#define INPUT_DATA (INPUT_CACHE[(CACHE_INDEX - 1 + 10) % 10])

static int system_running = 1;

// printS (or prints(test)) 
// prints stuff to screen
void PrintRawString(const char *Str) {
    while (*Str != '\0') {
        *UART_DATA = *Str;
        Str++;
    }
}

void prints(const char *Format, ...) {
    va_list Args;
    va_start(Args, Format);

    while (*Format != '\0') {
        if (*Format == '%' && *(Format + 1) != '\0') {
            Format++; 
            
            if (*Format == 's') {
                char *S = va_arg(Args, char*);
                PrintRawString(S);
            } 
            else if (*Format == 'd') {
                int N = va_arg(Args, int);
                if (N == 0) {
                    *UART_DATA = '0';
                } else {
                    char NumBuf[12];
                    int I = 0;
                    int IsNeg = 0;
                    if (N < 0) {
                        IsNeg = 1;
                        N = -N;
                    }
                    while (N > 0) {
                        NumBuf[I++] = (N % 10) + '0';
                        N /= 10;
                    }
                    if (IsNeg) {
                        NumBuf[I++] = '-';
                    }
                    while (I > 0) {
                        *UART_DATA = NumBuf[--I];
                    }
                }
            }
        } else {
            *UART_DATA = *Format;
        }
        Format++;
    }
    va_end(Args);
}

// inputS (or also inputs(text))
// inputs stuff in screen
void inputs(const char *prompt_text) {
    prints(prompt_text); // dear me dont delete prints or inputs wont run at all

    if (CACHE_INDEX >= 10) {
        CACHE_INDEX = 0;
    }

    char *buffer = INPUT_CACHE[CACHE_INDEX];
    int index = 0;

    while (index < 63) {
        while (*UART_FR & (1 << 4));
        char key = (char)(*UART_DATA);
        
        if (key != 0) {
            *UART_DATA = key;

            if (key == '\r' || key == '\n') {
                break;
            } if (key == '\b' || key == 127) {
                if (index > 0) {
                    index--;
                    prints("\b \b");
                }
            }
            buffer[index] = key;
            index++;
        }
    }
    
    buffer[index] = '\0';
    CACHE_INDEX = (CACHE_INDEX + 1) % 10;
}

// These things
void *kmemset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

void *memset(void *s, int c, size_t n) {
    return kmemset(s, c, n);
}

// basically like cmp x0, x1, outputintegername
// eggs
#define str(str1, str2, output) (output = strings_match(str1, str2))

int strings_match(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (*s1 == *s2);
}

// detects CPU
// self explanatory
static char CPUType[64] = "NULL";

void DetectCPU(void) {
    uint64_t Midr;
    asm volatile("mrs %0, midr_el1" : "=r" (Midr));

    uint16_t PartNum = (Midr >> 4) & 0xFFF;
    const char *SrcStr = "";

    switch(PartNum) {
        case 0xD03: 
            SrcStr = "ARM Cortex-A53";
            break;
        case 0xD07: 
            SrcStr = "ARM Cortex-A57";
            break;
        case 0xD08: 
            SrcStr = "ARM Cortex-A72";
            break;
        default: 
            SrcStr = "Generic AArch64";
            break;
    }

    int Index = 0;
    while (SrcStr[Index] != '\0' && Index < 63) {
        CPUType[Index] = SrcStr[Index];
        Index++;
    }
    CPUType[Index] = '\0';
}


int MainCoolThingy(void) {
    // dear me, this is the main kernel
    // Well technically one im confused
    // This whole thing is just built around 
    // Inputting, printing, booting, profitting
    // Yes.
    // don't be a dumb guy and not understand this

    DetectCPU();
    prints("\033[2J\033[H");
    prints("Welcome to Rhienel!\n");
    prints("======================\n\n");
    prints("[sys] init\n");
    prints("[sys] CPU found: %s\n", CPUType);

    int HELP = 0;
    int CLEAR = 0;
    int KILLINIT = 0;
    
    while (system_running == 1) {
        inputs("\nsys # ");

        str(INPUT_DATA, "help", HELP);
        str(INPUT_DATA, "clear", CLEAR);
        str(INPUT_DATA, "exit", KILLINIT);
        
        if (HELP == 1) {
            prints("\nhelp    Show this window\n");
            prints("clear   Clear screen\n");
            prints("exit    Exit main session\n");
        } else if (CLEAR == 1) {
            prints("\033[2J\033[H");           
        } else if (KILLINIT == 1) {
            prints(HALTMSG);
            return 0;
        } else {
            prints("\nIncorrect syntax");
        }
    
       // refresh, clear stuff like listerine mint mouthwash
        HELP = 0;
        CLEAR = 0;
        KILLINIT = 0;
    }

    return 0;
}

void _defeatTheKernel(void) {
    MainCoolThingy();
    for (;;) {
    }
}
}
