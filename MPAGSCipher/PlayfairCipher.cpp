// Standard library includes
#include <string>
#include <algorithm>
#include <cctype>

// Out project headers
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher( const std::string& key )
{
    PlayfairCipher::setKey(key);
}

void PlayfairCipher::setKey( const std::string& key )
{
    // store the original key
    key_ = key;

    // Append the alphabet
    key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Make sure the key is upper case
    std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);

    // Remove non-alpha characters
    auto func1 = [] (char in_char)
    {
        return(!std::isalpha(in_char));
    };
    auto iter = std::remove_if(key_.begin(), key_.end(), func1);
    key_.erase(iter, key_.end());

    // Change J -> I
    auto func2 = [] (char in_char)
    {
        if(in_char=='J') {return('I');}
        else {return(in_char);}
    };
    std::transform(key_.begin(), key_.end(), key_.begin(), func2);

    // Remove duplicated letters
    std::string enc_lett;
    auto func3 = [&] (char in_char)
    {
        if( enc_lett.find(in_char) == std::string::npos)
        {
            enc_lett += in_char;
            return(false);
        }
        else
        {
            return(true);
        }
    };
    auto iter2 = std::remove_if(key_.begin(), key_.end(), func3);
    key_.erase(iter2, key_.end());

    // Store the coords of each letter

    // Store the playfair chipher key map
}


std::string PlayfairCipher::applyCipher( const std::string& inputText, const CipherMode cipherMode ) const
{

    // Create the output string
    std::string outputText {inputText};

    // Change J -> I
    auto func3 = [] (char in_char)
    {
        if(in_char=='J') return('I');
        else return(in_char);
    };
    std::transform(outputText.begin(), outputText.end(), outputText.begin(), func3);

    // If repeated chars in a digraph add an X or Q if XX
    for( size_t i=1; i<outputText.size(); i+=2)
    {
        if(outputText[i]==outputText[i-1])
        {
            outputText.insert(i, ( (outputText[i]=='X')?("Q"):("X")) );
        }
    }

    // if the size of input is odd, add a trailing Z

    if(outputText.size()%2 != 0)
    {
        outputText += (outputText.back() == 'Z') ? 'X' : 'Z';
    }

    // Loop over the input in Digraphs
    for( size_t i=0; i<outputText.size(); i+=2)
    {
        //   - Find the coords in the grid for each digraph
        size_t index1 = key_.find(outputText[i]);
        size_t index2 = key_.find(outputText[i+1]);

        size_t row1 = index1/5;
        size_t row2 = index2/5;

        size_t column1 = index1%5;
        size_t column2 = index2%5;

        //   - Apply the rules to these coords to get 'new' coords
        if( row1 == row2 )
        {
            // Row - so increment/decrement the column indices (modulo 5)
            if(cipherMode==CipherMode::Encrypt)
            {
                column1 = (column1 + 1) % 5;
                column2 = (column2 + 1) % 5;
            }
            else
            {
                column1 = (5 + column1 - 1) % 5;
                column2 = (5 + column2 - 1) % 5;
            }
        }      
        else if( column1 == column2  )
        {
            // Column - so increment/decrement the row indices (modulo 5)
            if(cipherMode==CipherMode::Encrypt)
            {
                row1 = (row1 + 1) % 5;
                row2 = (row2 + 1) % 5;
            }
            else
            {
                row1 = (5 + row1 - 1) % 5;
                row2 = (5 + row2 - 1) % 5;
            }
        }
        else
        {
            // Rectangle/Square - so keep the rows the same and swap the columns
            std::swap( column1, column2 );
        }

        //   - Find the letter associated with the new coords
        index1 = 5*row1 + column1;
        index2 = 5*row2 + column2;

        //   - Make the replacements
        outputText[i]   = key_[index1];
        outputText[i+1] = key_[index2];
    }

    // return the text
    return outputText;
}
