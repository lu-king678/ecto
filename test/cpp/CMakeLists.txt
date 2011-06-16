find_package(GTest)

add_executable(ecto-test
  main.cpp
  tendril.cpp
  )

target_link_libraries(ecto-test
  ecto
  ${GTEST_LIBRARIES}
  ${ECTO_DEP_LIBS}
  )

add_test(gtest_units ${EXECUTABLE_OUTPUT_PATH}/ecto-test)
