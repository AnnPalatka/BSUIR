
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>


int command_succeeded = 0;

void interrupt(*old_kb_handler)(...);
void interrupt new_kb_handler(...);

void set_mask(unsigned int mask) {
  int i = 0;

  while (!command_succeeded) {
    // Wait until buffer is empty
    while ((inp(0x64) & 0x02));
    outp(0x60, 0xED);

    while ((inp(0x64) & 0x02));
    outp(0x60, mask);

    if (++i == 3) {
      fputs("Failed to set mask 3 times in a row\n", stderr);
      setvect(0x09, old_kb_handler);
      exit(EXIT_FAILURE);
    }
  }
  command_succeeded = 0;
}

// SCROLL_LOCK 0x01
// NUM_LOCK 0x02
// CAPS_LOCK 0x04

void light_kboard()
{
	for(int i = 1; i < 6; i++)
	{
		set_mask(i);
		delay(1000);
	}
	
	set_mask(0);
}

int main(void) {
	old_kb_handler = getvect(0x09);
	setvect(0x09, new_kb_handler);

	light_kboard();
 
	setvect(0x09, old_kb_handler);
	return EXIT_SUCCESS;
}


void interrupt new_kb_handler(...) 
{
	unsigned int scan_code = inp(0x60);
	printf("%X  ", scan_code);

	if (scan_code == 0xFA)// successful read
	{
		command_succeeded = 1;
	}
	else 
	{
		command_succeeded = 0;
	}

	old_kb_handler();
}