/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bryan Chacosky - 2013 'Crafter' Code Sample
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Crafter.h"
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static inline void checkRecipe( Crafter< > *crafter, std::array< CraftingObjectInterface *, 9 > *ingredients );
static inline std::array< CraftingObjectInterface *, 9 > torchIngredients( void );
static inline std::array< CraftingObjectInterface *, 9 > torchIngredientsWithNewLocation( void );
static inline std::array< CraftingObjectInterface *, 9 > fenceIngredients( void );
static inline std::array< CraftingObjectInterface *, 9 > invalidIngredients( void );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main( int, char ** )
{
  std::array< CraftingObjectInterface *, 9 > ingredientLists[ ] =
  {
    torchIngredients( ),
    torchIngredientsWithNewLocation( ),
    fenceIngredients( ),
    invalidIngredients( )
  };

  // Create a new crafter:
  Crafter< > *crafter = new Crafter< >( );

  // Register the valid recipes:
  crafter->addRecipe( &ingredientLists[ 0 ], CreateCraftingObjectAllocator( Torch ) );
  //crafter->addRecipe( &ingredientLists[ 1 ], CreateCraftingObjectAllocator( Torch ) );    // This will throw an assertion since we already have this recipe!
  crafter->addRecipe( &ingredientLists[ 2 ], CreateCraftingObjectAllocator( Fence ) );

  // Check each recipe:
  printf( "Checking TORCH recipe... " );                        checkRecipe( crafter, &ingredientLists[ 0 ] );
  printf( "Checking TORCH WITH NEW LOCATION recipe... " );      checkRecipe( crafter, &ingredientLists[ 1 ] );
  printf( "Checking FENCE recipe... " );                        checkRecipe( crafter, &ingredientLists[ 2 ] );
  printf( "Checking INVALID recipe... " );                      checkRecipe( crafter, &ingredientLists[ 3 ] );

  // Cleanup:
  delete crafter;

  printf( "\n" );
  system( "pause" );
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper methods:

static inline void checkRecipe( Crafter< > *crafter, std::array< CraftingObjectInterface *, 9 > *ingredients )
{
  CraftingObjectAllocator *allocator  = crafter->interpretIngredients( ingredients );
  bool                     exists     = allocator != NULL;

  if ( exists == true )
  {
    CraftingObjectInterface *createdObject = ( *allocator )( );

    printf( "Successfully created item with hash value '%c'!\n", createdObject->hashValue( ) );

    delete createdObject;
  }
  else
  {
    printf( "No item found!\n" );
  }
}

static inline std::array< CraftingObjectInterface *, 9 > torchIngredients( void )
{
  std::array< CraftingObjectInterface *, 9 > ingredients =
  {
    NULL, NULL, NULL,
    NULL, new Coal( ), NULL,
    NULL, new Stick( ), NULL
  };

  // Ignore leaks for the sample - real application will pull objects from the interface which means they already exist:
  return ingredients;
};

static inline std::array< CraftingObjectInterface *, 9 > torchIngredientsWithNewLocation( void )
{
  std::array< CraftingObjectInterface *, 9 > ingredients =
  {
    new Coal( ), NULL, NULL,
    new Stick( ), NULL, NULL,
    NULL, NULL, NULL
  };

  // Ignore leaks for the sample - real application will pull objects from the interface which means they already exist:
  return ingredients;
};

static inline std::array< CraftingObjectInterface *, 9 > fenceIngredients( void )
{
  std::array< CraftingObjectInterface *, 9 > ingredients =
  {
    new Stick( ), new Stick( ), new Stick( ),
    new Stick( ), new Stick( ), new Stick( ),
    NULL, NULL, NULL
  };

  // Ignore leaks for the sample - real application will pull objects from the interface which means they already exist:
  return ingredients;
}

static inline std::array< CraftingObjectInterface *, 9 > invalidIngredients( void )
{
  std::array< CraftingObjectInterface *, 9 > ingredients =
  {
    new Stick( ), new Stick( ), new Stick( ),
    new Stick( ), NULL, new Stick( ),
    NULL, NULL, NULL
  };

  // Ignore leaks for the sample - real application will pull objects from the interface which means they already exist:
  return ingredients;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////