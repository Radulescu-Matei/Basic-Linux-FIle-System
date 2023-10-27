#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

// This function initializes an empty ListNode in order to
// be used in other functions.
ListNode *createNode(TreeNode *parent, char *name, int type)
{
    ListNode *new_node = malloc(sizeof(ListNode));
    if (new_node == NULL)
        exit(-1);
    new_node->next = NULL;

    TreeNode *aux = malloc(sizeof(TreeNode));
    if (aux == NULL)
        exit(-1);

// The functions receives the file or folder's parent
// and it's name and saves them
    aux->parent = parent;
    aux->name = malloc(strlen(name) + 1);
    if (aux->name == NULL)
        exit(-1);

    memcpy(aux->name, name, strlen(name) + 1);
    aux->content = NULL;

// The function receives the number 0 or 1 in order
// to know wether a folder or a file is being created
    if (type == 0)
    {
        aux->type = FILE_NODE;
    }
    else
    {
        aux->type = FOLDER_NODE;
    }
    new_node->info = aux;

    return new_node;
}

// This function creates the FileTree
FileTree createFileTree(char *rootFolderName)
{
// The function creates a new FileTree
// and initializes it's root node
    FileTree new;
    new.root = malloc(sizeof(TreeNode));
    if (new.root == NULL)
        exit(-1);

    new.root->name = rootFolderName;
    new.root->type = FOLDER_NODE;
    new.root->content = malloc(sizeof(List));
    if (new.root->content == NULL)
        exit(-1);

// The function that creates an empty list and insert's
// it in the root's node content pointer.
    List new_list;
    new_list.head = NULL;
    memcpy(new.root->content, &new_list, sizeof(List));

    return new;
}

// This function frees the memory allocated for the FileTree
void freeTree(FileTree fileTree)
{
    TreeNode *root = fileTree.root;
    List *content = root->content;
    ListNode *aux = content->head;

// The function goes through each element saved in the root
// node and use the function rmrec to delete each one as it deletes
// and frees all elements no matter what they contain inside
// them
    while (aux != NULL)
    {
        rmrec(root, aux->info->name);
        aux = content->head;
    }

// Than the list that it's in the root and the root itself
// are freed
    free(content);
    content = NULL;

    free(root);
    root = NULL;
}

// This function frees one node from a List,
// more specifically speaking the TreeNode inside it
void free_node(ListNode *removed)
{
// The fucntion frees every part of the TreeNode
// and than gives them the NULL value to avoid invalid
// free problems.
    TreeNode *info = removed->info;
    free(info->name);
    info->name = NULL;
    free(info->content);
    info->content = NULL;
    free(info);
    info = NULL;
}

// This function lists the files and directories in the given
// directory, or the content of a file if the parameter it's a file
// if no parameter is given it provides such a list for the current
// directory
void ls(TreeNode *currentNode, char *arg)
{
    TreeNode *node;
    List *display;
    ListNode *aux;
    display = currentNode->content;
    aux = display->head;

// If the fucntion doesn't have an argument it ignores this
    if (strcmp(arg, NO_ARG) != 0)
    {
    // The function looks for the given file/directory.
        while (aux != NULL)
        {
            if (strcmp(aux->info->name, arg) == 0)
            {
                break;
            }
            aux = aux->next;
        }
    // If the file/directory is not found it prints the following message.
        if (aux == NULL)
        {
            printf("ls: cannot access '%s': No such file or directory", arg);
            return;
        }

    // If a file is found it prints it's name and content
        if (aux->info->type == FILE_NODE)
        {
            if (aux->info->content == NULL)
            {
                printf("%s:", aux->info->name);
            }
            printf("%s: %s", aux->info->name, (char *)aux->info->content);
            return;
        }
        display = aux->info->content;
        aux = display->head;
    }
// If a directory is found or no paremeter is given it simply
// prints each element in the required folder.
    while (aux != NULL)
    {
        printf("%s\n", aux->info->name);
        aux = aux->next;
    }
}

// This function prints the path to the current directory
void pwd(TreeNode *treeNode)
{
// When the root is found it prints root
    if (treeNode->parent == NULL)
    {
        printf("root");
        return;
    }
// Otherwise it recursively calls itself and than prints the actual
// current node.
    pwd(treeNode->parent);
    printf("/%s", treeNode->name);
}

// This functions returns the Node obtained by following
// a given path from the currentNode.
TreeNode *cd(TreeNode *currentNode, char *path)
{
    char *currentpath;
    currentpath = strtok(path, "/");
// This function uses strtok to separate the name of
// each directory.
    TreeNode *futureNode = currentNode;

    while (currentpath != NULL)
    {
    // If the parameter .. is given we go back in
    // the parent of the current folder
        if (strcmp(currentpath, "..") == 0)
        {
            futureNode = futureNode->parent;
        }
        else
        {
        // Otherwise the function looks for the given parameter
            List *currList = futureNode->content;
            ListNode *aux = currList->head;
            while (aux)
            {
                if (strcmp(aux->info->name, currentpath) == 0)
                {
                    break;
                }
                aux = aux->next;
            }
        // If the element is not found or the element is a file
        // the following message will be printed.
            if (aux == NULL || aux->info->type == FILE_NODE)
            {
                printf("cd: no such file or directory: %s\n", path);
                return currentNode;
            }
            futureNode = aux->info;
        }
        currentpath = strtok(NULL, "/");
    }

    // The functions returns the node that was found.
    currentNode = futureNode;
    return currentNode;
}

// This functions prints recursively the tree of a given directory
// and indents them by hierachy using a given parameter. It also
// counts the total number of files and directories.
void treeshow(TreeNode *currentNode, int *dir, int *fil, int indent)
{
    List *content = currentNode->content;
    ListNode *aux = content->head;

// This loop prints each element of the List that contains
// the elements of the directory, having to cases
// for files or folders, in case it is a folder it
// calls itself again.
    while (aux != NULL)
    {
        if (aux->info->type == FILE_NODE)
        {
            for (int i = 0; i < indent; i++)
                printf("    ");
            printf("%s\n", aux->info->name);
            ++(*fil);
        }
        if (aux->info->type == FOLDER_NODE)
        {
            for (int i = 0; i < indent; i++)
                printf("    ");
            printf("%s\n", aux->info->name);
            TreeNode *next = aux->info;
            treeshow(next, dir, fil, indent + 1);
            ++(*dir);
        }
        aux = aux->next;
    }
}

// This function checks the error cases listed and follows the
// given path. Afterwards it calls the treeshow function.
void tree(TreeNode *currentNode, char *arg)
{
    char *currentpath;
    char *oldarg = arg;
    currentpath = strtok(arg, "/");
    // This function uses strtok to separate the name of
    // each directory.

    while (currentpath != NULL)
    {
        // If the parameter .. is given we go back in
        // the parent of the current folder
        if (strcmp(currentpath, "..") == 0)
        {
            currentNode = currentNode->parent;
        }
        else
        {
            // Otherwise the function looks for the given parameter
            List *currList = currentNode->content;
            ListNode *aux = currList->head;
            while (aux)
            {
                if (strcmp(aux->info->name, currentpath) == 0)
                {
                    if (aux->info->type == FOLDER_NODE)
                    {
                        break;
                    }
                }
                aux = aux->next;
            }

            // In case the directory is not found the following
            // message is printed:
            if (aux == NULL)
            {
                printf("%s [error opening dir]\n\n0 directories, 0 files\n"
                , oldarg);
                return;
            }
            currentNode = aux->info;
        }
        currentpath = strtok(NULL, "/");
    }

// After the errors cases have been showed the treeshow function is called
// with it's parameters starting at 0.
    int dir = 0, fil = 0, indent = 0;
    treeshow(currentNode, &dir, &fil, indent);
    printf("%d directories, %d files", dir, fil);
}

// This function creates and empty directory
void mkdir(TreeNode *currentNode, char *folderName)
{
    List *currentList = currentNode->content;
    ListNode *aux = currentList->head;

// If it finds that the directory exists already it prints
// the message below.
    while (aux != NULL)
    {
        if (strcmp(aux->info->name, folderName) == 0)
        {
            printf("mkdir: cannot create directory '%s': File exists"
            , folderName);
            return;
        }
        aux = aux->next;
    }
    aux = currentList->head;

// Using the createNode function it creats a Directory with the
// name given as a parameter
    ListNode *newnode = createNode(currentNode, folderName, 1);

// If the lists head is empty it simply makes the newnode the head
// otherwise it puts the other elements after it before making it the
// head.
    if (aux == NULL)
    {
        currentList->head = newnode;
    }
    else
    {
        newnode->next = currentList->head;
        currentList->head = newnode;
    }

// This part creates a list and adds it in the content of the directory's
// info field.
    List new_list;
    new_list.head = NULL;
    TreeNode *info = newnode->info;
    info->content = malloc(sizeof(List));
    if (info->content == NULL)
        exit(-1);

    memcpy(info->content, &new_list, sizeof(List));
}

// This function removes a file or director wether it's empty or not.
void rmrec(TreeNode *currentNode, char *resourceName)
{
    TreeNode *node;
    List *display;
    ListNode *aux, *prev;
    display = currentNode->content;
    aux = display->head;

// The function looks for the element that should be deleted
    while (aux != NULL)
    {
        if (strcmp(aux->info->name, resourceName) == 0)
        {
            break;
        }
        prev = aux;
        aux = aux->next;
    }

// If the element is not found it prints the following message.
    if (aux == NULL)
    {
        printf("rmrec: failed to remove '%s': No such file or directory"
        , resourceName);
        return;
    }

// If the element is a file it deletes using the rm function.
    if (aux->info->type == FILE_NODE)
    {
        rm(currentNode, aux->info->name);
        return;
    }

// If the element is an empty directory it deletes it using
// the rmdir function.
    List *content = aux->info->content;
    if (content->head == NULL)
    {
        rmdir(currentNode, aux->info->name);
        return;
    }

// If the element is a non empty folder the function calls
// itself recursivly untill the folder has become empty
// as a reuslt of other nodes being deleted.
    ListNode *aux2 = content->head;
    while (aux2 != NULL)
    {
        rmrec(aux->info, aux2->info->name);
        aux2 = content->head;
    }

// In the end as the folder is empty it deletes it with rmdir.
    rmdir(currentNode, resourceName);
}

// This function delets a file
void rm(TreeNode *currentNode, char *fileName)
{
    TreeNode *node;
    List *display;
    ListNode *aux, *prev;
    display = currentNode->content;
    aux = display->head;

// The function looks for the file through the current List
    while (aux != NULL)
    {
        if (strcmp(aux->info->name, fileName) == 0)
        {
            break;
        }
        prev = aux;
        aux = aux->next;
    }

// If the file is not found it's prints the following message
    if (aux == NULL)
    {
        printf("rm: failed to remove '%s': No such file or directory"
        , fileName);
        return;
    }

// If the given name is that of a directory the following messae is
// returned.
    if (aux->info->type == FOLDER_NODE)
    {
        printf("rm: cannot remove '%s': Is a directory", fileName);
        return;
    }

// If the file was the head in it's list that the head is changed
// otherwise the preious element which has been saved in the prev
// variable simply jumps a step to the next element in order to ignore
// the one that needs to be removed
    if (aux != display->head)
        prev->next = aux->next;
    else
    {
        display->head = aux->next;
    }

// Using the free_node function the node is emptied and than the node itself
// is freed.
    free_node(aux);
    free(aux);
    aux = NULL;
}

// This function deletes an empty directory
void rmdir(TreeNode *currentNode, char *folderName)
{
    TreeNode *node;
    List *display;
    ListNode *aux, *prev;
    display = currentNode->content;
    aux = display->head;

// The function looks for the directory through the list
    while (aux != NULL)
    {
        if (strcmp(aux->info->name, folderName) == 0)
        {
            break;
        }
        prev = aux;
        aux = aux->next;
    }

// If the dirctory is not found it prints the followin message.
    if (aux == NULL)
    {
        printf("rmdir: failed to remove '%s': No such file or directory"
        , folderName);
        return;
    }

// If the given name is that of a file it prints the following message.
    if (aux->info->type == FILE_NODE)
    {
        printf("rmdir: failed to remove '%s': Not a directory", folderName);
        return;
    }

// If the directory is not empty it prints the following message.
    List *content = aux->info->content;
    if (content->head != NULL)
    {
        printf("rmdir: failed to remove '%s': Directory not empty", folderName);
        return;
    }

// The element is removed the same as in the rm function.
    if (aux != display->head)
        prev->next = aux->next;
    else
    {
        display->head = aux->next;
    }

// The memory is freed the same as in the rm function.
    free_node(aux);
    free(aux);
    aux = NULL;
}

// This function creates a File in the current directory.
void touch(TreeNode *currentNode, char *fileName, char *fileContent)
{
    List *currentList = currentNode->content;
    ListNode *aux = currentList->head;

// The function checks if the file already exists and in that
// case simply does nothing.
    while (aux != NULL)
    {
        if (strcmp(aux->info->name, fileName) == 0)
        {
            return;
        }
        aux = aux->next;
    }
    aux = currentList->head;

// The function uses the createNode function to make a new
// filetype node.
    ListNode *newnode = createNode(currentNode, fileName, 0);
// If the function has been given a content as a paremeter
// than it adds that content to the node.
    if (strcmp(fileContent, NO_ARG) != 0)
    {
        newnode->info->content = malloc(strlen(fileContent) + 1);
        if (newnode->info->content == NULL)
            exit(-1);
        memcpy(newnode->info->content, fileContent, strlen(fileContent) + 1);
    }

// If there are no elements in the list it makes the newnode the head.
    if (currentList->head == NULL)
    {
        currentList->head = newnode;
        return;
    }

// Otherwise it adds the old list after the new element and than makes
// it the head.
    newnode->next = currentList->head;
    currentList->head = newnode;
}

// This function copies a file to a given location.
void cp(TreeNode *currentNode, char *source, char *destination)
{
    TreeNode *srcNode, *destNode;
    char *currentpath, *olddest;
    olddest = malloc(strlen(destination) + 1);
    if (olddest == NULL)
        exit(-1);
    // The value of the destination path is saved in a copy so it
    // can be printed later.
    memcpy(olddest, destination, strlen(destination) + 1);
    currentpath = strtok(source, "/");
    TreeNode *futureNode = currentNode;
    // The function goes through the given path the same as in the cd
    // and tree function.
    while (currentpath != NULL)
    {
        if (strcmp(currentpath, "..") == 0)
        {
            futureNode = futureNode->parent;
        }
        else
        {
            List *currList = futureNode->content;
            ListNode *aux = currList->head;
            while (aux)
            {
                if (strcmp(aux->info->name, currentpath) == 0)
                {
                    break;
                }
                aux = aux->next;
            }
            futureNode = aux->info;
        }
        currentpath = strtok(NULL, "/");
    }
    // A directory cannot be copied so the following message is shown:
    if (futureNode->type == FOLDER_NODE)
    {
        printf("cp: -r not specified; omitting directory '%s'", source);
        free(olddest);
        return;
    }
    srcNode = futureNode;
    currentpath = strtok(destination, "/");
    futureNode = currentNode;
    int findlast = 1;
    char *name = NULL;

    // The function goes through the destination path the same as above.
    while (currentpath != NULL)
    {
        // If the hierarchy does not exist the following message is printed:
        if (findlast == 0)
        {
            printf("cp: failed to access '%s': Not a directory", olddest);
            free(olddest);
            return;
        }
        if (strcmp(currentpath, "..") == 0)
        {
            futureNode = futureNode->parent;
        }
        else
        {
            List *currList = futureNode->content;
            ListNode *aux = currList->head;
            while (aux)
            {
                if (strcmp(aux->info->name, currentpath) == 0)
                {
                    break;
                }
                aux = aux->next;
            }
            if (aux == NULL)
            {
                name = currentpath;
                findlast = 0;
            }
            else
            {
                // If a file is found the content is replaced
                if (aux->info->type == FILE_NODE)
                {
                    futureNode = aux->info;
                    break;
                }
                futureNode = aux->info;
            }
        }
        currentpath = strtok(NULL, "/");
    }
    destNode = futureNode;

    if (destNode->type == FOLDER_NODE && name == NULL)
    {
        // If a name is not given it takes the one from
        // the source path.
        if (srcNode->content != NULL)
        {
            char *auxcontent = srcNode->content;
            touch(destNode, srcNode->name, auxcontent);
        }
        else
        {
            touch(destNode, srcNode->name, NO_ARG);
        }
    }
    else
    {
        // If a name is provided it creates a file with
        // the new name
        if (name)
        {
            touch(destNode, name, srcNode->content);
        }
        else
        {
            // If the file already exists it's
            // content is replaced.
            if (srcNode->content != NULL)
            {
                char *extra = srcNode->content;
                destNode->content = malloc(strlen(extra) + 1);
                if (destNode->content == NULL)
                    exit(-1);
                memcpy(destNode->content, extra, strlen(extra) + 1);
            }
            else
            {
                destNode->content = NULL;
            }
        }
    }
    free(olddest);
}

// This function moves a file or a directory to a given location.
void mv(TreeNode *currentNode, char *source, char *destination)
{
    // The function saves the previous element and the list
    // in which each element is saved.
    ListNode *prev, *destprev, *srclistnode, *destlistnode;
    List *lastlist, *destlastlist;
    TreeNode *srcNode, *destNode;
    char *currentpath, *olddest;
    // Same copy as in cp.
    olddest = malloc(strlen(destination) + 1);
    if (olddest == NULL)
        exit(-1);
    memcpy(olddest, destination, strlen(destination) + 1);

    currentpath = strtok(source, "/");
    TreeNode *futureNode = currentNode;

// The function goes through the path the same way as the cd
// and tree function.
    while (currentpath != NULL) {
        if (strcmp(currentpath, "..") == 0) {
            futureNode = futureNode->parent;
        } else {
            List *currList = futureNode->content;
            ListNode *aux = currList->head;
            while (aux) {
                if (strcmp(aux->info->name, currentpath) == 0) {
                    break;
                }
                prev = aux;
                aux = aux->next;
            }
            futureNode = aux->info;
            lastlist = currList;
            srclistnode = aux;
        }
        currentpath = strtok(NULL, "/");
    }

    srcNode = futureNode;
    currentpath = strtok(destination, "/");
    futureNode = currentNode;

    int findlast = 1;
    char *name = NULL;

    while (currentpath != NULL) {
        if (findlast == 0) {
            printf("mv: failed to access '%s': Not a directory", olddest);
            free(olddest);
            return;
        }
        if (strcmp(currentpath, "..") == 0) {
            futureNode = futureNode->parent;
        } else {
            List *currList = futureNode->content;
            ListNode *aux = currList->head;
            while (aux) {
                if (strcmp(aux->info->name, currentpath) == 0) {
                    break;
                }
                destprev = aux;
                aux = aux->next;
            }
            if (aux == NULL) {
                name = currentpath;
                findlast = 0;
            } else {
            if (aux->info->type == FILE_NODE) {
                futureNode = aux->info;
                break;
            }
            futureNode = aux->info;
            destlistnode = aux;
            }
            destlastlist = currList;
        }
        currentpath = strtok(NULL, "/");
    }

    destNode = futureNode;

    // If a file has been given a parameter, and
    // the destination is a folder the file is created.
    if (srcNode->type == FILE_NODE) {
        if (destNode->type == FOLDER_NODE && name == NULL) {
            if (srcNode->content != NULL) {
                char *auxcontent = srcNode->content;
                touch(destNode, srcNode->name, auxcontent);
            } else {
                touch(destNode, srcNode->name, NO_ARG);
            }
        } else {
            // If the file does not already exists it is
            // created.
            if (name) {
                touch(destNode, name, srcNode->content);
            } else {
                if (srcNode->content != NULL) {
                    char *extra = srcNode->content;
                    destNode->content = malloc(strlen(extra) + 1);
                    if (destNode->content == NULL)
                        exit(-1);
                    memcpy(destNode->content, extra, strlen(extra) + 1);
                } else {
                    destNode->content = NULL;
                }
            }
        }
        // This removes the moved file.
        rm(cd(srcNode, ".."), srcNode->name);
    } else {
        // In case of the directory the element is
        // simply removed from the old list by
        // changing it's location pointers(next)
        // and added to the new one, special conditions
        // are put in place in case the head of a list has
        // to be changed.
        if (lastlist->head != srclistnode) {
            prev->next = prev->next->next;
        } else {
            lastlist->head = srclistnode->next;
        }
        List *currentl = destNode->content;
        srclistnode->next = currentl->head;
        currentl->head = srclistnode;
    }
    free(olddest);
}
