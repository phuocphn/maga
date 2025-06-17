import pytest
from unittest.mock import MagicMock, patch
from src.pymaga.examples.utils import print_circuit


@pytest.fixture
def mock_terminal():
    t = MagicMock()
    t.getIdentifier.return_value = "T1"
    t.getNet.return_value = "N1"
    return t


@pytest.fixture
def mock_instance_terminal():
    t = MagicMock()
    t.getIdentifier.return_value = "IT1"
    t.getNet.return_value = "IN1"
    return t


@pytest.fixture
def mock_instance(mock_instance_terminal):
    inst = MagicMock()
    inst.getInstanceTerminalMap.return_value = {"it1": mock_instance_terminal}
    inst.hasMaster.return_value = False
    inst.__str__.return_value = "Instance1"
    return inst


@pytest.fixture
def mock_circuit(mock_terminal, mock_instance):
    circ = MagicMock()
    circ.getTerminalMap.return_value = {"t1": mock_terminal}
    circ.getInstanceMap.return_value = {"i1": mock_instance}
    circ.__str__.return_value = "Circuit1"
    return circ


@pytest.fixture
def mock_tree():
    tree = MagicMock()
    tree.create_node = MagicMock()
    tree.show = MagicMock(return_value="tree shown")
    return tree


def test_print_circuit_basic(mock_circuit, mock_tree):
    # Patch Tree so no real treelib.Tree is used
    with patch("pymaga.examples.utils.Tree", return_value=mock_tree):
        result = print_circuit(mock_circuit)
        assert result == "tree shown"
        # Root node created
        mock_tree.create_node.assert_any_call(
            "Circuit1[#instances=1,#ports=1]: [T1(N1)]",
            "Circuit1[#instances=1,#ports=1]: [T1(N1)]",
        )
        # Instance node created
        mock_tree.create_node.assert_any_call(
            "Instance1[#ports=1]: [IT1(IN1)]",
            identifier="Instance1[#ports=1]: [IT1(IN1)]",
            parent="Circuit1[#instances=1,#ports=1]: [T1(N1)]",
        )
        mock_tree.show.assert_called_once()


def test_print_circuit_with_parent_and_print_level(mock_circuit, mock_tree):
    with patch("pymaga.examples.utils.Tree", return_value=mock_tree):
        # Simulate recursive call with parent and print_level
        print_circuit(mock_circuit, tree_obj=mock_tree, parent="parent", print_level=2)
        # Node created with parent
        mock_tree.create_node.assert_any_call(
            "Circuit1[#instances=1,#ports=1]: [T1(N1)]",
            "parentCircuit1[#instances=1,#ports=1]: [T1(N1)]",
            parent="parent",
        )


def test_print_circuit_instance_with_master(mock_circuit, mock_instance, mock_tree):
    # Setup instance to have a master
    master_circuit = MagicMock()
    master_circuit.getTerminalMap.return_value = {}
    master_circuit.getInstanceMap.return_value = {}
    master_circuit.__str__.return_value = "MasterCircuit"
    mock_instance.hasMaster.return_value = True
    mock_instance.getMaster.return_value = master_circuit

    with patch("pymaga.examples.utils.Tree", return_value=mock_tree):
        print_circuit(mock_circuit)
        # Should call create_node for master as well
        assert mock_tree.create_node.call_count >= 3  # root, instance, master


def test_print_circuit_print_level_100(mock_circuit, mock_tree):
    with patch("pymaga.examples.utils.Tree", return_value=mock_tree):
        # Should skip node creation when print_level == 100
        print_circuit(mock_circuit, tree_obj=mock_tree, print_level=100)
        mock_tree.create_node.assert_not_called()


def test_print_circuit_show_print_outputs(capsys, mock_circuit, mock_tree):
    with patch("pymaga.examples.utils.Tree", return_value=mock_tree):
        print_circuit(mock_circuit, show_print=True)
        captured = capsys.readouterr()
        assert "Circuit1[#instances=1,#ports=1]: [T1(N1)]" in captured.out
        assert "Instance1[#ports=1]: [IT1(IN1)]" in captured.out
