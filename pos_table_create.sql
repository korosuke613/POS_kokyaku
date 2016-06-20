--
--   customer_info table
--
CREATE TABLE customer_info(
	customer_id		integer,
	customer_name		text,
	customer_gender		boolean,
	customer_birth		date,
	customer_address 	text,
	customer_phone		char(12),
	customer_email		text,
	point_id		varchar(10),
	PRIMARY KEY (customer_id)
);

--
--   customer_point table
--
CREATE TABLE customer_point(
	customer_id		integer references customer_info(customer_id),
	hold_point		integer,
	pre_amount		integer,
	thism_amount		integer
);

