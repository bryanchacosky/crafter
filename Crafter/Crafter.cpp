/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bryan Chacosky - 2013 'Crafter' Code Sample
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Template-class instantiation file --- MUST exclude from build!                                                                                      */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CraftingObjects.h"
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lifecycle methods:

template < unsigned int Size >
Crafter< Size >::Crafter( void )
{
  // Initialize member variables:
  this->availableRecipes = new Crafter::RecipeMap( );
}

template < unsigned int Size >
Crafter< Size >::~Crafter( void )
{
  // Free member variables:
  delete this->availableRecipes;
  this->availableRecipes = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public methods:

template < unsigned int Size >
void Crafter< Size >::addRecipe( const std::array< CraftingObjectInterface *, Size * Size > * const ingredients, CraftingObjectAllocator allocator )
{
  assert( ingredients != NULL && ingredients->size( ) == Size * Size );   // Ingredients must be non-null and the expected size
  assert( allocator != NULL );                                            // Allocator must be non-null

  // Convert the ingredients list into a hashable string:
  std::string hashString = this->hashIngredients( ingredients );

  // Make sure that the hash key is valid and we don't already have an existing allocator for this recipe hash:
  assert( hashString.empty( ) == false );
  assert( this->availableRecipes->find( hashString ) == this->availableRecipes->end( ) );

  // Update our hash map:
  this->availableRecipes->insert( std::make_pair( hashString, allocator ) );
}

template < unsigned int Size >
CraftingObjectAllocator * const Crafter< Size >::interpretIngredients( const std::array< CraftingObjectInterface *, Size * Size > * const ingredients ) const
{
  // Convert the ingredients list into a hashable string:
  std::string hashString = this->hashIngredients( ingredients );
  if ( hashString.empty( ) == false )
  {
    // Check our hash map of available recipes:
    Crafter::RecipeMap::iterator iterator = this->availableRecipes->find( hashString );

    // Confirm that the iterator is valid:
    if ( iterator == this->availableRecipes->end( ) )
      return NULL;

    return &iterator->second;
  }

  return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private methods:

// Returns the trimmed hash key for the given ingredients.  By trimming the hash key, we get the added support of making the receipe independent of location
// in the crafting table without having to provide the content for each potential location:
template < unsigned int Size >
std::string Crafter< Size >::hashIngredients( const std::array< CraftingObjectInterface *, Size * Size > * const ingredients )
{
  assert( ingredients != NULL && ingredients->size( ) == Size * Size );   // Ingredients must be non-null and the expected size

  std::string hashString     = std::string( );    // Mutable string which will create the hash value
  int         lastValidIndex = -1;                // Index of the last valid entry ( used for trimming )

  for ( unsigned i = 0; i != ingredients->size( ); ++i )
  {
    CraftingObjectInterface *objectInterface = ingredients->at( i );

    if ( objectInterface != NULL )
    {
      // Add the object's hash value:
      hashString.push_back( objectInterface->hashValue( ) );

      // Update the last valid index:
      lastValidIndex = hashString.size( ) - 1;
    }
    else if ( lastValidIndex != -1 )
    {
      // Object is invalid, but we have seen at least one valid character so we need to append a nil character:
      hashString.push_back( Crafter::NilHashValue );
    }
  }

  if ( lastValidIndex != -1 )
  {
    // There was at least one valid character, so we need to strip any trailing Nil values:
    hashString.erase( lastValidIndex + 1, hashString.length( ) - lastValidIndex );
  }
  else
  {
    // No valid characters so hash value isn't valid:
    return std::string( );
  }

  return hashString;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////