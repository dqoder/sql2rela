# Translator - SQL to Relation algebra

Develop a translator which shall translate a given select statement of SQL into
	an equivalent expression in _relational algebra_. Your translator must include
	__SELECTION__, __PROJECTION__, __JOIN__ and __SET__ operation.

> Note: no subqueries

### examples

```
SELECT A, B
	FROM r
	WHERE A > 100
```

$\Pi_{A,B}(\sigma_{A>100}(r))$

```
SELECT *
	FROM r
	WHERE A > 100
```

`sigma_{A>100}(r)`
$\sigma_{A>100}(r)$

```
SELECT *
	FROM r JOIN s
	WHERE A > 100
```

$\sigma_{A>100}(r \Join s)$

```
SELECT A
	FROM r
INTERSECT
SELECT A
	FROM s
```

$\Pi_{A}$(r) $\cap$ $\Pi_{A}$(s)


## Plan

lexical analysic $\rightarrow$ parsing $\rightarrow$ substitution

## symbols

| SQL operation   | relational algebra |
| ---             |        ---         |
| SELECTION       |  $\sigma$          |
| PROJECTION      |  $\Pi$             |
| UNION           |  $\cup$            |
| INTERSECTION    |  $\cap$            |
| SET DIFFERENCE  |  $\minus$          |
| LEFT JOIN       |  $\leftouterjoin$  |
| RIGHT JOIN      |  $\rightouterjoin$ |
| FULL OUTER JOIN |  $\fullouterjoin$  |
| INNER      JOIN |  $\bowtie$         |

## grammar

__S__ $\rightarrow$ __S<sub>0</sub>__ | __S<sub>0</sub> T<sub>set</sub> S<sub>0</sub>__

__S<sub>0</sub>__ $\rightarrow$ __T<sub>s</sub> A T<sub>f</sub> R T<sub>w</sub> C__

__R__ $\rightarrow$ __T__
