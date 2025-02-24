#include "podio/UserDataCollection.h"
#include "podio/CollectionBufferFactory.h"
#include "podio/CollectionBuffers.h"

#include <tuple>
#include <vector>

namespace podio {

namespace {
  /**
   * Helper function to register a UserDataCollection to the
   * CollectionBufferFactory. Takes the BasicType as template argument.
   *
   * Returns an integer so that it can be used with std::apply
   */
  template <typename T>
  int registerUserDataCollection(T) {
    // Register with schema version 1 to allow for potential changes
    CollectionBufferFactory::mutInstance().registerCreationFunc(
        userDataCollTypeName<T>(), UserDataCollection<T>::schemaVersion, [](bool) {
          return podio::CollectionReadBuffers{new std::vector<T>(), nullptr, nullptr,
                                              [](podio::CollectionReadBuffers buffers, bool) {
                                                return std::make_unique<UserDataCollection<T>>(
                                                    std::move(*buffers.dataAsVector<T>()));
                                              },
                                              [](podio::CollectionReadBuffers& buffers) {
                                                buffers.data = podio::CollectionWriteBuffers::asVector<T>(buffers.data);
                                              }};
        });

    return 1;
  }

  /**
   * Helper function to loop over all types in the SupportedUserDataTypes to
   * register the UserDataCollection types.
   */
  bool registerUserDataCollections() {
    // Use an IILE here to make sure to do the call exactly once
    const static auto reg = []() {
      std::apply([](auto... x) { std::make_tuple(registerUserDataCollection(x)...); }, SupportedUserDataTypes{});
      return true;
    }();
    return reg;
  }

  /**
   * Invoke the registration function for user data collections at least once
   */
  const auto registeredUserData = registerUserDataCollections();
} // namespace

} // namespace podio
