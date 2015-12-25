/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef WASMINT_JUDYARRAY_H
#define WASMINT_JUDYARRAY_H

#ifndef JUDY64_H
#define JUDY64_H
//  Judy arrays 13 DEC 2012 (judy64n.c from http://code.google.com/p/judyarray/ )
//  This code is public domain.

//  Author Karl Malbrain, malbrain AT yahoo.com
//  with assistance from Jan Weiss.
//  modifications (and any bugs) by Mark Pictor, mpictor at gmail

//  Simplified judy arrays for strings and integers
//  Adapted from the ideas of Douglas Baskins of HP.

//  Map a set of keys to corresponding memory cells (uints).
//  Each cell must be set to a non-zero value by the caller.

//  String mappings are denoted by calling judy_open with zero as
//  the second argument.  Integer mappings are denoted by calling
//  judy_open with the Integer depth of the Judy Trie as the second
//  argument.

//  functions:
//  judy_open:  open a new judy array returning a judy object.
//  judy_close: close an open judy array, freeing all memory.
//  judy_clone: clone an open judy array, duplicating the stack.
//  judy_data:  allocate data memory within judy array for external use.
//  judy_cell:  insert a string into the judy array, return cell pointer.
//  judy_strt:  retrieve the cell pointer greater than or equal to given key
//  judy_slot:  retrieve the cell pointer, or return NULL for a given key.
//  judy_key:   retrieve the string value for the most recent judy query.
//  judy_end:   retrieve the cell pointer for the last string in the array.
//  judy_nxt:   retrieve the cell pointer for the next string in the array.
//  judy_prv:   retrieve the cell pointer for the prev string in the array.
//  judy_del:   delete the key and cell for the current stack entry.



#if defined(__LP64__)      || \
    defined(__x86_64__)    || \
    defined(__amd64__)     || \
    defined(_WIN64)        || \
    defined(__sparc64__)   || \
    defined(__arch64__)    || \
    defined(__powerpc64__) || \
    defined (__s390x__)
//    defines for 64 bit
typedef unsigned long long judyvalue;
typedef unsigned long long JudySlot;
#define JUDY_key_mask (0x07)
#define JUDY_key_size 8
#define JUDY_slot_size 8
#define JUDY_span_bytes (3 * JUDY_key_size)
#define JUDY_span_equiv JUDY_2
#define JUDY_radix_equiv JUDY_8

#define PRIjudyvalue    "llu"

#else
//    defines for 32 bit
typedef unsigned int judyvalue;
typedef unsigned int JudySlot;
#define JUDY_key_mask (0x03)
#define JUDY_key_size 4
#define JUDY_slot_size 4
#define JUDY_span_bytes (7 * JUDY_key_size)
#define JUDY_span_equiv JUDY_4
#define JUDY_radix_equiv JUDY_8

#define PRIjudyvalue    "u"

#endif

#define JUDY_mask (~(JudySlot)0x07)

//    define the alignment factor for judy nodes and allocations
//    to enable this feature, set to 64

#define JUDY_cache_line 64     // minimum size is 8 bytes
// can be calculated using http://stackoverflow.com/a/4049562/382458 - but that would limit optimization!
// 10x 1M key hexsort, line size 64: 9.949s; size 8: 10.018s --> 1% improvement for 64; however, this may be dwarfed by the sort code

#define JUDY_seg    65536

enum JUDY_types {
    JUDY_radix        = 0,    // inner and outer radix fan-out
    JUDY_1            = 1,    // linear list nodes of designated count
    JUDY_2            = 2,
    JUDY_4            = 3,
    JUDY_8            = 4,
    JUDY_16           = 5,
    JUDY_32           = 6,
#ifdef ASKITIS
    JUDY_64           = 7
#else
    JUDY_span         = 7     // up to 28 tail bytes of key contiguously stored
#endif
};

typedef struct {
    void * seg;               // next used allocator
    unsigned int next;        // next available offset
} JudySeg;

typedef struct {
    JudySlot next;            // judy object
    unsigned int off;         // offset within key
    int slot;                 // slot within object
} JudyStack;

typedef struct {
    JudySlot root[1];         // root of judy array
    void ** reuse[8];         // reuse judy blocks
    JudySeg * seg;            // current judy allocator
    unsigned int level;       // current height of stack
    unsigned int max;         // max height of stack
    unsigned int depth;       // number of Integers in a key, or zero for string keys
    JudyStack stack[1];       // current cursor
} Judy;

#ifdef ASKITIS
int Words = 0;
int Inserts = 0;
int Found = 0;
#  if JUDY_key_size < 8
#    define JUDY_max    JUDY_16
#  else
#    define JUDY_max    JUDY_64
#  endif
#else
#  define JUDY_max    JUDY_32
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// open a new judy array returning a judy object.
Judy * judy_open( unsigned int max, unsigned int depth );

/// close an open judy array, freeing all memory.
void judy_close( Judy * judy );

/// clone an open judy array, duplicating the stack.
Judy * judy_clone( Judy * judy );

/// allocate data memory within judy array for external use.
void * judy_data( Judy * judy, unsigned int amt );

/// insert a key into the judy array, return cell pointer.
JudySlot * judy_cell( Judy * judy, const unsigned char * buff, unsigned int max );

/// retrieve the cell pointer greater than or equal to given key
JudySlot * judy_strt( Judy * judy, const unsigned char * buff, unsigned int max );

/// retrieve the cell pointer, or return NULL for a given key.
JudySlot * judy_slot( Judy * judy, const unsigned char * buff, unsigned int max );

/// retrieve the string value for the most recent judy query.
unsigned int judy_key( Judy * judy, unsigned char * buff, unsigned int max );

/// retrieve the cell pointer for the last string in the array.
JudySlot * judy_end( Judy * judy );

/// retrieve the cell pointer for the next string in the array.
JudySlot * judy_nxt( Judy * judy );

/// retrieve the cell pointer for the prev string in the array.
JudySlot * judy_prv( Judy * judy );

/// delete the key and cell for the current stack entry.
JudySlot * judy_del( Judy * judy );

#ifdef __cplusplus
}
#endif

#endif //JUDY64_H

#ifndef JUDYLARRAY_H
#define JUDYLARRAY_H

/****************************************************************************//**
* \file judyLArray.h C++ wrapper for judyL array implementation
*
* A judyL array maps JudyKey's to corresponding memory cells, each containing
* a JudyValue. Each cell must be set to a non-zero value by the caller.
*
*    Author: Mark Pictor. Public domain.
*
********************************************************************************/

#include "assert.h"

#ifdef HAVE_STD_ENABLEIF
#include <type_traits>
#endif

template< typename JudyKey, typename JudyValue >
struct judylKVpair {
    JudyKey key;
    JudyValue value;
};

/** A judyL array maps JudyKey's to corresponding memory cells, each containing
 * a JudyValue. Each cell must be set to a non-zero value by the caller.
 *
 * Both template parameters must be the same size as a void*
 *  \param JudyKey the type of the key, i.e. uint64_t, pointer-to-object, etc
 *  \param JudyValue the type of the value
 */
template< typename JudyKey, typename JudyValue >
class judyLArray {
public:
    typedef judylKVpair< JudyKey, JudyValue > pair;
protected:
    Judy * _judyarray;
    unsigned int _maxLevels, _depth;
    JudyValue * _lastSlot;
    JudyKey _buff[1];
    bool _success;
    pair _kv;
public:
    judyLArray(): _maxLevels( sizeof( JudyKey ) ), _depth( 1 ), _lastSlot( 0 ), _success( true ) {
        assert( sizeof( JudyKey ) == JUDY_key_size && "JudyKey *must* be the same size as a pointer!" );
        assert( sizeof( JudyValue ) == JUDY_key_size && "JudyValue *must* be the same size as a pointer!" );
        _judyarray = judy_open( _maxLevels, _depth );
        _buff[0] = 0;
    }

    explicit judyLArray( const judyLArray< JudyKey, JudyValue > & other ): _maxLevels( other._maxLevels ),
                                                                           _depth( other._depth ), _success( other._success ) {
        _judyarray = judy_clone( other._judyarray );
        _buff[0] = other._buff[0];
        find( *_buff ); //set _lastSlot
    }

    ~judyLArray() {
        judy_close( _judyarray );
    }

    ///empty the judy array, delete nothing
    ///overload below can also delete JudyValue's, iff they are a pointer type
    void clear() {
        JudyKey key = 0;
        while( 0 != ( _lastSlot = ( JudyValue * ) judy_strt( _judyarray, ( const unsigned char * ) &key, 0 ) ) ) {
            judy_del( _judyarray );
        }
    }
#ifdef HAVE_STD_ENABLEIF
    template <typename X=JudyValue>
        typename std::enable_if<std::is_pointer<X>::value, void>::type
        clear( bool deleteContents ) {
            JudyKey key = 0;
            while( 0 != ( _lastSlot = ( JudyValue * ) judy_strt( _judyarray, ( const unsigned char * ) &key, 0 ) ) ) {
                if( deleteContents ) {
                    delete *_lastSlot;
                }
                judy_del( _judyarray );
            }
        }
#endif

    JudyValue getLastValue() {
        assert( _lastSlot );
        return &_lastSlot;
    }

    void setLastValue( JudyValue value ) {
        assert( _lastSlot );
        &_lastSlot = value;
    }

    bool success() {
        return _success;
    }
    //TODO
    // allocate data memory within judy array for external use.
    // void *judy_data (Judy *judy, unsigned int amt);

    /// insert or overwrite value for key
    bool insert( JudyKey key, JudyValue value ) {
        assert( value != 0 );
        _lastSlot = ( JudyValue * ) judy_cell( _judyarray, ( const unsigned char * ) &key, _depth * JUDY_key_size );
        if( _lastSlot ) {
            *_lastSlot = value;
            _success = true;
        } else {
            _success = false;
        }
        return _success;
    }

    /// retrieve the cell pointer greater than or equal to given key
    /// NOTE what about an atOrBefore function?
    const pair atOrAfter( JudyKey key ) {
        _lastSlot = ( JudyValue * ) judy_strt( _judyarray, ( const unsigned char * ) &key, _depth * JUDY_key_size );
        return mostRecentPair();
    }

    /// retrieve the cell pointer, or return NULL for a given key.
    JudyValue find( JudyKey key ) {
        _lastSlot = ( JudyValue * ) judy_slot( _judyarray, ( const unsigned char * ) &key, _depth * JUDY_key_size );
        if( _lastSlot ) {
            _success = true;
            return *_lastSlot;
        } else {
            _success = false;
            return 0;
        }
    }

    /// retrieve the key-value pair for the most recent judy query.
    inline const pair & mostRecentPair() {
        judy_key( _judyarray, ( unsigned char * ) _buff, _depth * JUDY_key_size );
        if( _lastSlot ) {
            _kv.value = *_lastSlot;
            _success = true;
        } else {
            _kv.value = ( JudyValue ) 0;
            _success = false;
        }
        _kv.key = _buff[0];
        return _kv;
    }

    /// retrieve the first key-value pair in the array
    const pair & begin() {
        JudyKey key = 0;
        _lastSlot = ( JudyValue * ) judy_strt( _judyarray, ( const unsigned char * ) &key, 0 );
        return mostRecentPair();
    }

    /// retrieve the last key-value pair in the array
    const pair & end() {
        _lastSlot = ( JudyValue * ) judy_end( _judyarray );
        return mostRecentPair();
    }

    /// retrieve the key-value pair for the next key in the array.
    const pair & next() {
        _lastSlot = ( JudyValue * ) judy_nxt( _judyarray );
        return mostRecentPair();
    }

    /// retrieve the key-value pair for the prev key in the array.
    const pair & previous() {
        _lastSlot = ( JudyValue * ) judy_prv( _judyarray );
        return mostRecentPair();
    }

    /** delete a key-value pair. If the array is not empty,
     * getLastValue() will return the entry before the one that was deleted
     * \sa isEmpty()
     */
    bool removeEntry( JudyKey * key ) {
        if( judy_slot( _judyarray, key, _depth * JUDY_key_size ) ) {
            _lastSlot = ( JudyValue * ) judy_del( _judyarray );
            return true;
        } else {
            return false;
        }
    }

    /// true if the array is empty
    bool isEmpty() {
        JudyKey key = 0;
        return ( ( judy_strt( _judyarray, ( const unsigned char * ) &key, _depth * JUDY_key_size ) ) ? false : true );
    }
};
#endif //JUDYLARRAY_H

#endif //WASMINT_JUDYARRAY_H
