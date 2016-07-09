--
--  fare_info table
--	フェア情報
CREATE TABLE fair_info(
	fair_id		integer,
	fair_name	varchar(100),
	fair_start	date,
	fair_end	date,
	PRIMARY KEY (fair_id)
);	



--
--  big_category table
--	カテゴリ大
CREATE TABLE big_category(
	category_big_id	varchar(2),
	category_big_name	varchar(64),
	PRIMARY KEY (category_big_id)
);

--
--  category_detail table
--	カテゴリ詳細	
CREATE TABLE category_detail(
	category_small_id	integer,
	category_big_id		varchar(2) references big_category(category_big_id),
	category_small_name	varchar(64),
	PRIMARY KEY (category_small_id)
);

--
--  country_info table
--	国情報
CREATE TABLE country_info(
	country_id		integer,	
	country		text,
	PRIMARY KEY (country_id)
);
--
--  region_info table
--	地域情報
CREATE TABLE region_info(
	area_id		integer,
	area			text,
	country_id		integer references country_info(country_id),
	country		text,
	PRIMARY KEY (area_id)
);

--
--  customer_info table
--	顧客情報
CREATE TABLE customer_info(
	customer_id		integer,
	customer_name		text,
	customer_gender	boolean,
	customer_birth	date,
	customer_address	text,
	customer_phone	char(12),
	customer_email	text,
	point_id		integer,
	PRIMARY KEY (customer_id)
);

--
--  customer_point table
--	顧客ポイント
CREATE TABLE customer_point(
	customer_id		integer references customer_info(customer_id),
	hold_point		integer,
	pre_month_amount		integer,
	this_month_amount		integer
);

--
--	point_high_low
--	高頻度利用者及び通常利用者のポイント率
CREATE TABLE point_high_low(
	month_amount	integer,
	point_rate double precision,
	PRIMARY KEY (month_amount)
);

--
--  store_info table
--	店舗
CREATE TABLE store_info(
	store_id	integer,
	store_name		text,
	store_category		text,
	area_id		integer references region_info(area_id),
	store_address		text,
	store_tel		varchar(15),
	store_time		varchar(15),
	PRIMARY KEY (store_id)
);

--
--	tax_rate table
--	税率
CREATE TABLE tax_rate(
	category_id		integer references category_detail(category_small_id),
	country_id			integer references country_info(country_id),
	tax_rate		double precision
);


--
--  prod_info table
--	商品情報
CREATE TABLE prod_info(
	goods_id	integer,
	goods_name	varchar(100),
	category_id	integer references category_detail(category_small_id),
	cost		integer,
	PRIMARY KEY (goods_id)
);



--
--  purc_info table
--	仕入れ情報
CREATE TABLE purc_info(
	purchase_id	integer,
	store_id	integer references store_info(store_id),
	purchase_date	date,
	total_value	integer,
	PRIMARY KEY (purchase_id)
);

--
--  purc_detail table
--	仕入れ詳細
CREATE TABLE purc_detail(
	purchase_id		integer references purc_info(purchase_id),
	goods_id		integer references prod_info(goods_id),
	goods_value		integer,
	goods_quantity	integer,
	price_value		integer,
	limit_date		date
);

--
--  sale_info table
--	販売情報
CREATE TABLE sale_info(
	sell_id		integer,
	customer_id		integer references customer_info(customer_id),
	store_id		integer references store_info(store_id),
	area_id		integer references region_info(area_id),
	amount_s		integer,
	tax_s			integer,
	weather		varchar(10),
	temperature	integer,
	date	date,
	time		time,
	PRIMARY KEY (sell_id)	
);

--
--  fair_detail table
--  フェア詳細
CREATE TABLE fair_detail(
	fair_id   integer references fair_info(fair_id),
	goods_id  integer references prod_info(goods_id),
	point_rate  double precision
);

--
--  sale_detail table
--	販売詳細
CREATE TABLE sale_detail(
	sell_id		integer references sale_info(sell_id),
	goods_id		integer references prod_info(goods_id),
	amount			integer,
	price			integer,
	tax_m			integer
);


--
--  store_prod table
--	店舗取扱い詳細
CREATE TABLE store_prod(
	store_id		integer references store_info(store_id),
	goods_id		integer references prod_info(goods_id),
	goods_num		integer,
	start_date		date,
	end_date		date
);


--
--  send_counter table
--	送信回数
CREATE TABLE send_counter(
	store_id			integer references store_info(store_id),
	m_address		text,
	send_counter		integer
);
