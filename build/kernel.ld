SECTIONS
{
    . = 0xAE00 + SIZEOF_HEADERS;
    .text :
    {
        *(.text)
    }
}