DEFINE-CHECKER TEST_PATCH_0002 = {
  LET method_has_a_parameter_with_name(x) =
        WHEN HOLDS-NEXT WITH-TRANSITION ParameterName x
        HOLDS-IN-NODE FunctionDecl;
  SET report_when =
           method_has_a_parameter_with_type("Pippo");
  SET message = "Found a function with a parameter named Pippo";
};
