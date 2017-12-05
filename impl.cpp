// CPSC457 Fall 2017, University of Calgary
// Skeleton C++ program for Q7 of Assignment 5.
//
// The program reads in the input, then calls the (wrongly implemented) checkConsistency()
// function, and finally formats the output.
//
// You only need to reimplement the checkConsistency() function.
//
// Author: Pavol Federl (pfederl@ucalgary.ca or federl@gmail.com)
// Date: November 29, 2017
// Version: 1

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

typedef std::string SS;
typedef std::vector<SS> VS;

struct DEntry {
    SS fname = SS( 4096, 0);
    int size = 0;
    int ind = 0;
    bool tooManyBlocks = false;
    bool tooFewBlocks = false;
    bool hasCycle = false;
    bool sharesBlocks = false;
};

static SS join( const VS & toks, const SS & sep) {
    SS res;
    bool first = true;
    for( auto & t : toks) { res += (first ? "" : sep) + t; first = false;}
    return res;
}

// re-implement this function
//
// Parameters:
//   blockSize - contains block size as read in from input
//   files - array containing the entries as read in from input
//   fat - array representing the FAT as read in from input
// Return value:
//   the function should return the number of free blocks
//   also, for ever entry in the files[] array, you need to set the appropriate flags:
//      i.e. tooManyBlocks, tooFewBlocks, hasCycle and sharesBlocks
int checkConsistency( int blockSize, std::vector<DEntry> & files, std::vector<int> & fat)
{
    int blocksize = blockSize;
    std::vector<DEntry> filesV = files;
    std::vector<int> fatV = fat;


    printf("FUCK");
    std::vector<int> visited;

    for(int i = 0; i<filesV.size();i++){
        DEntry file = files[i];
        int neededBlocks = ceil(file.size/(double)blockSize);
        printf("Needed blocks = %d\n", neededBlocks);
        printf("%d\n", file.ind);
        if(file.ind == -1){

        }else{

            int start = fatV.at(file.ind);
            int actualBlocks = 1;
            std::vector<int> selfVisited;

            while(start != -1){

                bool selfBeen = false;
                for(int a = 0; a<selfVisited.size(); a++){
                    if(selfVisited.at(a) == start){
                        selfBeen = true;
                    }
                }
                if (selfBeen){
                    files[i].hasCycle = true;
                    actualBlocks++;
                    break;
                }
                actualBlocks++;
                selfVisited.push_back(start);



                start = fatV.at(start);
            }



            // for(;;){
            //     if(start != -1){
                    
            //         bool selfBeen = false;
            //         for(int a = 0; a<selfVisited.size(); a++){
            //             if(selfVisited.at(a) == start){
            //                 selfBeen = true;
            //             }
            //         }

            //         if (selfBeen){
            //             file.hasCycle = true;
            //             break;
            //         }
            //         actualBlocks++;
            //         selfVisited.push_back(start);
            //         start = fatV.at(start);
            //     }else{
            //         break;
            //     }
            // }

            if(actualBlocks>neededBlocks){
                files[i].tooManyBlocks = true;
            }else if(actualBlocks<neededBlocks){
                files[i].tooFewBlocks = true;
            }

        }

    }





    // finally, return the number of free blocks
    return 0;
}

int main()
{
    try {
        // read in blockSize, nFiles, fatSize
        int blockSize, nFiles, fatSize;
        if( 3 != scanf( "%d %d %d", & blockSize, & nFiles, & fatSize))
            throw "cannot read blockSize, nFiles and fatSize";
        if( blockSize < 1 || blockSize > 1024) throw "bad block size";
        if( nFiles < 0 || nFiles > 50) throw "bad number of files";
        if( fatSize < 1 || fatSize > 200000) throw "bad FAT size";
        // read in the entries
        std::vector<DEntry> entries;
        for( int i = 0 ; i < nFiles ; i ++ ) {
            DEntry e;
            if( 3 != scanf( "%s %d %d", (char *) e.fname.c_str(), & e.ind, & e.size))
                throw "bad file entry";
            e.fname = e.fname.c_str();
            if( e.fname.size() < 1 || e.fname.size() > 16)
                throw "bad filename in file entry";
            if( e.ind < -1 || e.ind >= fatSize) throw "bad first block in fille entry";
            if( e.size < 0 || e.size > 1073741824) throw "bad file size in file entry";
            entries.push_back( e);
        }
        // read in the FAT
        std::vector<int> fat( fatSize);
        for( int i = 0 ; i < fatSize ; i ++ ) {
            if( 1 != scanf( "%d", & fat[i])) throw "could not read FAT entry";
            if( fat[i] < -1 || fat[i] >= fatSize) throw "bad FAT entry";
        }

        int nFreeBlocks = checkConsistency( blockSize, entries, fat);
        size_t maxflen = 0;
        for( auto & e : entries ) maxflen = std::max( maxflen, e.fname.size());
        SS fmt = "  %" + std::to_string( maxflen) + "s: %s\n";

        printf( "Issues with files:\n");
        for( auto & e : entries ) {
            VS issues;
            if( e.tooFewBlocks) issues.push_back( "not enough blocks");
            if( e.tooManyBlocks) issues.push_back( "too many blocks");
            if( e.hasCycle) issues.push_back( "contains cycle");
            if( e.sharesBlocks) issues.push_back( "shares blocks");
            printf( fmt.c_str(), e.fname.c_str(), join( issues, ", ").c_str());
        }
        printf( "Number of free blocks: %d\n", nFreeBlocks);

    }
    catch( const char * err) {
        fprintf( stderr, "Error: %s\n", err);
    }
    catch( ... ) {
        fprintf( stderr, "Errro: unknown.\n");
    }
    return 0;
}
