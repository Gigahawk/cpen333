#!/bin/bash
input=$(ls | grep "\.h$")
cmd="hpp2plantuml "
while IFS= read -r f; do
    cmd+="-i $f "
done <<< "$input"
cmd+="-o out.plantuml"
echo "$cmd"
eval "$cmd"
# Struct is not a valid plantuml type
sed -i 's/struct/entity/g' out.plantuml
plantuml out.plantuml
