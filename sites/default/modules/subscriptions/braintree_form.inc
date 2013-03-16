<h1>Braintree Credit Card Transaction Form</h1>
    <div>
      <form action="/create_transaction" method="POST" id="braintree-payment-form">
        <p>
          <label>Card Number</label>
          <input type="text" size="20" autocomplete="off" data-encrypted-name="number" />
        </p>
        <p>
          <label>CVV</label>
          <input type="text" size="4" autocomplete="off" data-encrypted-name="cvv" />
        </p>
        <p>
          <label>Expiration (MM/YYYY)</label>
          <input type="text" size="2" data-encrypted-name="month" /> / <input type="text" size="4" data-encrypted-name="year" />
        </p>
        <input type="submit" id="submit" />
      </form>
    </div>
    <script type="text/javascript" src="https://js.braintreegateway.com/v1/braintree.js"></script>
    <script type="text/javascript">
      var braintree = Braintree.create("MIIBCgKCAQEAs/hPHehk3Yq/utGpBsImUVXh0LDtFvM6/XJyNpMmjl3ZN9Hp/sftkI9gSIoS/zpIqSBQ+CK7SnyVjpmdq0wWshVZe2V+8dSySGnuj000rMIYTTUVwIecUAaIpNDE4CUh/3mSktUI2vAHcNzZ4RIQJFjaTFsOYQA0gp3WNilE28YahpnUVT1hkPZBvXkL2c4TF/dB6ucduv0627Hp42ks89IYWR2WvzxksjcME24fmo28UWviXraSR8MCGmCJ95VJ305oAXprgF7DPDQ/YBynAQ+K+GYQ2Rv1wrAaXyMILfpHL3SQ79IselfV43Zf9xuLN7/KmbnWFwi2XNi4/2KPvQIDAQAB");
      braintree.onSubmitEncryptForm("braintree-payment-form");
    </script>