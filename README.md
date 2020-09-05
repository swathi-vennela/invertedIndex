# Inverted Index
This is an implementation of inverted index, which maps the words extracted from a set of documents, to the documents in which they are present.

I've taken a set of documents, extracted words present in them, and then mapped each word with "posting lists" (list of documents in which the word occurs). For each word, I've stored the number of documents in which it occurs, and the number of times it occurs in each document.

## Datastructures chosen for implementation

<b>For words</b>  :  Trie datastructure
  
<b>For posting lists</b> :  Binary Search Tree

The leaf nodes of the trie, mark the end of word. These nodes will point to the posting list corresponding to
that word. 
