hr is a terminal horizontal rule.  Are you tired of losing your place in
terminal history due to pages and pages of nearly identical text?  Do you find
yourself pressing return multiple times just so you can find a chunk of empty
prompts in your history? hr might be for you!

## Building

See the INSTALL file

## How to use

hr, by default, will generate a horizontal rule across the terminal while
overwriting the prompt in which you invoked hr to begin with.  The string used
to display the rule can be changed by passing it as an argument:

    hr ":-) "

You can also change the default symbol with the HR_SYMBOL environment variable.
Assuming the bash shell:

    export HR_SYMBOL=":-) "

Finally, note that Unicode is supported, so you can have fun with box-drawing
characters and other symbols.  For instance, the following UTF-8 encoded example
renders a line of flowery asterisks:

    export HR_SYMBOL='✱ ✲ ✳ ❃ ❂ ❁ ❀ ✿ ✾ ✽ ✼ ✻ '

Or, a squiggly line:

    export HR_SYMBOL='°º¤ø,¸¸,ø¤º°`' 

## Support

Please send any comments and suggestions to Paul Giblock <p at pgiblock dot net>
