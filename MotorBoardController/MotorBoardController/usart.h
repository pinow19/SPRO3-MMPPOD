#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

void uart_init(void);
void io_redirect(void);


#endif

