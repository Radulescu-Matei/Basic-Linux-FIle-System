Radulescu Matei

Description of basic linux file system implementation:

1) createNode - This function crates a node and initializes it's parent and name values,
it saves wether it's a file or a folder depending on the value given as a parameter
which is 0 for a file and any other number for a folder.

2) freeNode - This function empties the treeNode inside of a ListNode so the LIstNode
can be freed afterwards, this is used for deleting elements.

3)treeshow - This is a function that prints the elements from a specified path recursively
and when the path is a directory, it calls the function again from the location of the
directory.


1) createFileTree: This function creates the FIleTree struct, in doing sow it initalizes it's root, it
also insert's an empty LIst as the root's content.

2)freeTree: This function frees the memory allocated for the tree, it does so using the rmrec
function, which deletes every element saved in the root node and frees it, after that only
the root itself has to be freed.

3)ls: This function shows the either the files and folder contained in a given directory, the content
of a given file, or the content of the current directory if no parameter is given.

4)pwd: This function traverses the tree recursively towards the root and when getting
there, it prints all the TreeNode's and thus printing them backwards from the TreeNode
up which actually prints them in the right way.

5)cd:  This function moves the currentNode by returning the node of the specified path
by using strtok to separate each directory in the path parameter and verifying for each
one if it exists and is accessible. It traverses the list and finds out if the path from
strtok is accessible and exists and if it doesn't it displays the needed error message.
When the while finishes, it means that the path is found and thus it changes the
current node and returns it.


6)tree: This function goes on the specified path (the algorithm for finding the path is the
same from the one in cd) and when arriving to it, calls the treeshow function to print
the tree from the path down. Lastly, the function prints how many directories and
files it has found by having them from the treeshow function.

7)mkdir: This function creates a directory using the createnode function, if a directory with the
same name already exists than it returns an error message. The directory is always added as
the first element in the list in which is saved.

8)touch: This function creates a file using the createnode function, if a file with the
same name already exists than it does nothing The file is always added as
the first element in the list in which is saved.

9)rmrec: This function removes a file or directory wether it contains anything or not. If it
find a file it removes it with rm and if it find and empty folder wth rmdir. If a non-empty folder
is found it calls itself recursivly for each element of the folder, which will remove all element using
rm and rmdir, and than uses rmdir to remove the now empty folder.

10)rm: This function removes a file, it uses the freeNode function to empty the LIstNode found,
and than frees the node itself.

11)rmdir: This function removes a directory, pretty much same way as the rm function as en empty
directory only conatins an empty least which can be freed with the same method as the elements in rm
are freed.

12)cp: This function copies a file from a source and sends a copy to its destination.
It firstly remembers the old destination so it can be used later in the error message.
Afterwards it uses the same path traversal as used before in cd and stores the
resulting nodes to be used later to copy the source node to the destination one
if needed. After getting the two nodes, it verifies if there are any errors and if there
are none, it goes in different cases depending if the file has contents, doesn't have
contents, if it has a name, if it exists and more.

13)mv: This function moves a file or a directory from a source and sends it to the
desired destination. Same as in the cp function, it remembers the old destination
to be used in the error messages and uses the path traversal from cd while also
remembering the previous, current and list of the source and destination path
so it can be used later. Finally, it finds out if the source is a file or a directory
and acts correspondently by creating the file and deleting it with rm and if
it's a directory, it doesn't delete anything but it modifies the next pointer in the
needed nodes by using the previously stored values the traversal and thus
the directories remain the same but gets moved in another list.
