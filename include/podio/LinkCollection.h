#ifndef PODIO_LINKCOLLECTION_H
#define PODIO_LINKCOLLECTION_H

#include "podio/detail/LinkCollectionImpl.h"
#include "podio/detail/PreprocessorMacros.h"

#ifndef PODIO_ENABLE_SIO
  #define PODIO_ENABLE_SIO 0
#endif

// __COUNTER__ is a widely-supported compiler extension (GCC, Clang, MSVC) but not standard C++.
// In Clang 22+ it warns as a C2y extension; suppress that at each expansion site.
#if defined( __clang__ ) && __clang_major__ >= 22
#  define PODIO_COUNTER_PRAGMA_PUSH \
    _Pragma( "clang diagnostic push" ) _Pragma( "clang diagnostic ignored \"-Wc2y-extensions\"" )
#  define PODIO_COUNTER_PRAGMA_POP _Pragma( "clang diagnostic pop" )
#else
#  define PODIO_COUNTER_PRAGMA_PUSH
#  define PODIO_COUNTER_PRAGMA_POP
#endif

/// Main macro for declaring links. Takes care of registering the necessary
/// buffer creation functionality with the CollectionBufferFactory.
#define PODIO_DECLARE_LINK(FromT, ToT)                                                                                 \
  PODIO_COUNTER_PRAGMA_PUSH                                             \
  const static auto PODIO_PP_CONCAT(REGISTERED_LINK_, __COUNTER__) =                                                   \
      podio::detail::registerLinkCollection<FromT, ToT>(podio::LinkCollection<FromT, ToT>::typeName); \
  PODIO_COUNTER_PRAGMA_POP

#if PODIO_ENABLE_SIO && __has_include("podio/detail/LinkSIOBlock.h")
  #include <podio/detail/LinkSIOBlock.h>
  /// Macro for registering the necessary SIOBlock for a Link with the SIOBlock factory
  #define PODIO_DECLARE_LINK_SIO(FromT, ToT)                                                                           \
  PODIO_COUNTER_PRAGMA_PUSH                                               \
    const static auto PODIO_PP_CONCAT(LINK_SIO_BLOCK_, __COUNTER__) = podio::LinkSIOBlock<FromT, ToT>{}; \
  PODIO_COUNTER_PRAGMA_POP
#endif

#endif // PODIO_LINKCOLLECTION_H
