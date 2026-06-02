<h1> NeoVIM learnig </h1>

- [Paragraphs](#paragraphs)
  - [To shift-indent (add/remove indentation to) an entire paragraph:](#to-shift-indent-addremove-indentation-to-an-entire-paragraph)
  - [To auto-indent/realign code within a paragraph (e.g., fix inconsistent indentation):](#to-auto-indentrealign-code-within-a-paragraph-eg-fix-inconsistent-indentation)
  - [To reformat/wrap a text paragraph (e.g., reflow to 'textwidth', often 80 columns):](#to-reformatwrap-a-text-paragraph-eg-reflow-to-textwidth-often-80-columns)


## Paragraphs

In **Neovim** (and Vim), a **paragraph** is defined as a block of text separated by blank lines.

### To shift-indent (add/remove indentation to) an entire paragraph:
- Place your cursor anywhere within the paragraph.
- In normal mode, type **`>ip`** to indent the inner paragraph right by one shiftwidth (usually 2–8 spaces/tabs, depending on your config).
- Type **`<ip`** to de-indent (shift left).
- Repeat with `.` (dot) for multiple shifts, or prefix with a count like `3>ip` for three levels.

For including the lines above/below if needed: **`>ap`** (around paragraph).

### To auto-indent/realign code within a paragraph (e.g., fix inconsistent indentation):
- Use **`={motion}`**, where the motion targets the paragraph.
- Common commands:
  - `=ip` — Auto-indent the inner paragraph (great for code blocks).
  - `=ap` — Auto-indent the paragraph including surrounding lines.
  - `vip=` — Visually select inner paragraph first, then `=` to re-indent.

### To reformat/wrap a text paragraph (e.g., reflow to 'textwidth', often 80 columns):
- `gqip` — Reformat inner paragraph (breaks lines at words, respects 'textwidth').
- `gwip` — Similar to gqip, but preserves cursor position and doesn't use external 'formatprg'.

First set `'textwidth'` if needed, e.g., `:set textwidth=80`.

These use built-in text objects (`ip` for inner paragraph, `ap` for a paragraph). No plugins required. For details, see `:help text-objects`, `:help >`, `:help =`, or `:help gq` in Neovim.

```C
/* practice snippet */
#undef DBJ_STRING_TYPE

#define DBJ_STRING_TYPE(SIZE_) \
                struct dbj_string_##SIZE_  \
    {                          \
char data[SIZE_];      \
                               }
```