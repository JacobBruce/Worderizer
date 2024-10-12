# Worderizer

Worderizer is a fast text tokenizer written in C++. Worderizer can build a vocabulary by scanning text files and tries to tokenize whole words instead of sub-words, where a word is a series of alphabetical characters. Requires [parallel-hashmap](https://github.com/greg7mdp/parallel-hashmap) (copy parallel_hashmap folder into the includes folder).

## USAGE

The following example shows how to generate a new vocabulary (aka a word map) for the tokenizer:

```
#include "Worderizer.h"

int main()
{
    phmap::parallel_flat_hash_map<std::u32string, uint32_t> words;

    Worderizer::MaxCharCode = 65536; // max character code
    Worderizer::MaxWordLen = 64; // max word length
    Worderizer::MinOccurr = 2; // min occurences to keep word
    
    // second argument is directory containing text files
    Worderizer::GenEnglishWordMap(words, "C:/text_files/");

    // tries to remove repetitive nonsense words
    Worderizer::CleanWordMap(words);

    // save word map to file
    Worderizer::SaveWordMap(words, "C:/wordmap.bin");
}
```

You can modify the Worderizer::IsAlpha() function to change the valid alphabetical characters. All other characters will be treated as single word but you can also add custom pairs of special characters to the word map:

```
#include "Worderizer.h"

int main()
{
    phmap::parallel_flat_hash_map<std::u32string, uint32_t> words;

    // word must be a pair of special characters or alphabetical word
    std::vector<std::u32string> newWords =
    {
        std::u32string(U"\n\n"),
        std::u32string(U". "),
        std::u32string(U"Hello")
    }

    // load word map from file
    Worderizer::LoadWordMap(words, "C:/wordmap.bin");

    // add new words to loaded word map
    Worderizer::AddWordsToMap(words, newWords);

    // save final word map to file
    Worderizer::SaveWordMap(words, "C:/wordmap.bin");
}
```

Once you have a word map it can be used to tokenize UTF32 text strings. Use the Worderizer::U32ToU8() and Worderizer::U8ToU32() functions to convert between UTF32 strings and UTF8 encoded strings (std::u32string <-> std::string).

```
#include "Worderizer.h"

int main()
{
    // allow console to print UTF8 text
    SetConsoleOutputCP(CP_UTF8);

    phmap::parallel_flat_hash_map<std::u32string, uint32_t> words;

    // string to be tokenized, U means UTF32 string literal
    std::u32string str(U"Hello world! Testing 123!");

    std::vector<uint32_t> tokens;

    Worderizer::LoadWordMap(words, "C:/wordmap.bin");

    // get tokens representing string
    Worderizer::StrToTokens(str, tokens, words);

    std::cout << "Tokens: ";

    for (const uint32_t& token : tokens)
        std::cout << " " << token;

    std::cout << std::endl;

    // convert tokens back into string
    Worderizer::TokensToStr(str, tokens, words);

    std::cout << "String: " << Worderizer::U32ToU8(str) << std::endl;
}
```