//Aidan Kelly
//10173966
//A5
//It's super slow. Just a warning.

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


//way to check if element is in vector.
bool isIn(std::vector<int> vect, int searching){

    bool found = false;

    for(int i = 0; i<vect.size(); i++){
        if(vect.at(i)==searching){
            return true;
        }
    }

    return false;
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

    //sets up the total used blocks
    int overallUsedBlocks = 0;


    //create vector to hold the overall visited blocks.
    std::vector<int> visited;

    //loop through the files
    for(int i = 0; i<filesV.size();i++){

        std::vector<int> personalVisited;

        //sets up the block counter.
        int actualBlocks = 0;
        
        //make a copy of file
        DEntry file = filesV.at(i);

        //calculate how many blocks are needed for the file
        int neededBlocks = ceil(file.size/(double)blockSize);

        //if file index is -1. means the file should be empty
        if(file.ind == -1){

            //if file isnt empty, not enough blocks
            if(neededBlocks > 0){
                files[i].tooFewBlocks = true;
            }

        //else file has blocks
        }else{

            //set up vectors to check for cycles and shared blocks

            int start = file.ind;
            while(start!=-1){
                if(start == -1){
                    break;
                }

                if(isIn(personalVisited, start)){
                    files[i].hasCycle = true;
                    break;
                }

                if(isIn(visited, start)){
                    files[i].sharesBlocks = true;
                }

                
                personalVisited.push_back(start);
                visited.push_back(start);
                start = fat.at(start);

                actualBlocks++;

            }

            if(actualBlocks > neededBlocks){
                files[i].tooManyBlocks = true;
            }else if(actualBlocks < neededBlocks){
                files[i].tooFewBlocks = true;
            }
            printf("\nFile %d", i);
            printf("Needed Blocks = %d Actual Blocks = %d", neededBlocks, actualBlocks);


            
            

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
