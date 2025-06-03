from treelib import Tree
import pymaga


def get_circuit_tree(
    circuit,
    tree_obj: Tree = None,
    parent: str = None,
    show_print: bool = False,
    print_level: int = 0,
) -> str:
    """
    Recursively prints and/or builds a tree representation of a circuit hierarchy.

    Args:
        circuit: The circuit object to be traversed. Must provide `getTerminalMap()`, `getInstanceMap()`, and string representation.
        tree_obj (optional): An instance of a Tree object used to build the hierarchy. If None, a new Tree is created at the root call.
        parent (optional): The parent node identifier in the tree. Used for recursive calls to maintain hierarchy.
        show_print (bool, optional): If True, prints the circuit and instance hierarchy to stdout. Defaults to False.
        print_level (int, optional): The current recursion depth, used for indentation and controlling print behavior. Defaults to 0.

    Returns:
        If called at the root level (print_level == 0), returns the result of `tree_obj.show(stdout=False)`, which return a string that displays the constructed tree.
        Otherwise, returns None.

    Notes:
        - The function traverses the circuit's instances recursively, printing and/or building a tree structure that represents the circuit hierarchy.
        - Each node in the tree includes information about the number of instances, ports, and their identifiers.
        - Assumes the existence of a Tree class with a `create_node` and `show` method.
    """

    if print_level == 0 and tree_obj is None:
        tree_obj = Tree()

    interface = [
        f"{terminal.getIdentifier()}({terminal.getNet()})"
        for terminal in circuit.getTerminalMap().values()
    ]
    interface = (
        f"[#instances={len(circuit.getInstanceMap().values())},#ports={len(circuit.getTerminalMap().values())}]: ["
        + ",".join(interface)
        + "]"
    )

    circuit_description = "📁" + str(circuit) + interface
    if print_level != 100:
        if show_print:
            print("  " * print_level + circuit_description)
        if parent is None:
            tree_obj.create_node(circuit_description, circuit_description)
        else:
            tree_obj.create_node(
                circuit_description,
                parent + circuit_description,
                parent=parent,
            )

    if len(circuit.getInstanceMap().values()) > 0:
        for instance in circuit.getInstanceMap().values():
            # try:
            #     instance_interface = [
            #         f"{instanceTerminal.getIdentifier()}({instanceTerminal.getNet()})"
            #         for instanceTerminal in instance.getInstanceTerminalMap().values()
            #     ]
            # except Exception as e:
            #     print(e)
            #     instance_interface = [
            #         f"{instanceTerminal.getIdentifier()}(<none>)"
            #         for instanceTerminal in instance.getInstanceTerminalMap().values()
            #     ]
            instance_interface = []
            for instanceTerminal in instance.getInstanceTerminalMap().values():
                try:
                    instance_interface.append(
                        f"{instanceTerminal.getIdentifier()}({instanceTerminal.getNet()})"
                    )
                except Exception as e:
                    print(e)
                    instance_interface.append(
                        f"{instanceTerminal.getIdentifier()}(<none>)"
                    )

            instance_interface = (
                f"[#ports={len(instance.getInstanceTerminalMap().values())}]: ["
                + ",".join(instance_interface)
                + "]"
            )
            if show_print:
                print(("  " * (print_level + 1)) + str(instance) + instance_interface)

            if parent is None:
                tree_obj.create_node(
                    str(instance) + instance_interface,
                    identifier=str(instance) + instance_interface,
                    parent=circuit_description,
                )
            else:
                tree_obj.create_node(
                    str(instance) + instance_interface,
                    identifier=parent + str(instance) + instance_interface,
                    parent=parent + circuit_description,
                )
            if instance.hasMaster():
                if parent is None:
                    parent_id = str(instance) + instance_interface
                else:
                    parent_id = parent + str(instance) + instance_interface

                get_circuit_tree(
                    instance.getMaster(),
                    tree_obj,
                    parent_id,
                    show_print,
                    print_level + 2,
                )
    if print_level == 0:
        return tree_obj.show(stdout=False)
